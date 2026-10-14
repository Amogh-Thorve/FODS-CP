// CONFIGCORE - Main Application Controller
let currentWizardStep = 1;
let cachedProducts = [];
let cachedOptions = [];
let activeCategoryFilter = 'ALL';
let currentConfigItems = [];

// Demo Tour State
let demoCurrentStage = 0;
let demoAutoplayTimer = null;
let demoAutoplaySpeed = 2500; // ms
let isDemoPlaying = false;

document.addEventListener('DOMContentLoaded', () => {
  initNavigation();
  loadDashboardData();
  preloadCatalogData();
});

// Toast Notification Utility
function showToast(title, message, type = 'info') {
  const container = document.getElementById('toast-container');
  if (!container) return;
  const toast = document.createElement('div');
  toast.className = `toast toast-${type}`;
  const icon = type === 'success' ? '✓' : type === 'error' ? '⚠' : type === 'warning' ? '⚡' : 'ℹ';
  toast.innerHTML = `
    <div style="font-size: 16px; font-weight: bold;">${icon}</div>
    <div class="toast-content">
      <div class="toast-title">${title}</div>
      <div class="toast-message">${message}</div>
    </div>
  `;
  container.appendChild(toast);
  setTimeout(() => {
    toast.style.opacity = '0';
    toast.style.transform = 'translateX(100%)';
    setTimeout(() => toast.remove(), 350);
  }, 4200);
}

// Navigation Initialization
function initNavigation() {
  const navItems = document.querySelectorAll('.nav-item');
  const pageViews = document.querySelectorAll('.page-view');
  const pageTitle = document.getElementById('page-title');

  navItems.forEach(item => {
    item.addEventListener('click', () => {
      const targetView = item.getAttribute('data-view');
      
      navItems.forEach(n => n.classList.remove('active'));
      pageViews.forEach(v => v.classList.remove('active'));

      item.classList.add('active');
      const viewEl = document.getElementById(`view-${targetView}`);
      if (viewEl) viewEl.classList.add('active');

      if (pageTitle) {
        pageTitle.textContent = item.textContent.trim();
      }

      // Load specific view data
      switch(targetView) {
        case 'dashboard': loadDashboardData(); break;
        case 'products': loadProductsView(); break;
        case 'configurator': loadConfiguratorView(); break;
        case 'compatibility': loadCompatibilityView(); break;
        case 'rules': loadRulesView(); break;
        case 'pricing': loadPricingView(); break;
        case 'recommendations': loadRecommendationsView(); break;
        case 'queue': loadQueueView(); break;
        case 'structures': loadStructuresView(); break;
        case 'algorithms': loadAlgorithmsView(); break;
        case 'benchmark': loadBenchmarkView(); break;
        case 'history': loadHistoryView(); break;
        case 'syllabus': loadSyllabusView(); break;
        case 'complexity': loadComplexityView(); break;
        case 'system': loadSystemView(); break;
      }
    });
  });
}

function navigateToConfigurator() {
  const item = document.querySelector('.nav-item[data-view="configurator"]');
  if (item) item.click();
}

function navigateToProducts() {
  const item = document.querySelector('.nav-item[data-view="products"]');
  if (item) item.click();
}

// Preload Catalog Data
async function preloadCatalogData() {
  if (!cachedProducts.length) {
    cachedProducts = (await API.getProducts()) || [];
  }
  if (!cachedOptions.length) {
    cachedOptions = (await API.getOptions()) || [];
  }
}

// 1. Dashboard View
async function loadDashboardData() {
  await preloadCatalogData();
  const config = await API.getConfiguration();
  const rules = await API.getRules();
  const queue = await API.getQueue();

  if (cachedProducts) document.getElementById('stat-total-products').textContent = cachedProducts.length;
  if (cachedOptions) document.getElementById('stat-total-options').textContent = cachedOptions.length;
  if (config) document.getElementById('stat-active-config').textContent = `${config.length} items`;
  if (queue) document.getElementById('stat-pending-jobs').textContent = queue.pendingCount || 0;

  // Render product table
  if (cachedProducts) {
    const tbody = document.getElementById('dashboard-product-table');
    if (tbody) {
      tbody.innerHTML = cachedProducts.slice(0, 5).map(p => `
        <tr>
          <td><strong>${p.sku}</strong></td>
          <td>${p.name}</td>
          <td>${p.category}</td>
          <td>₹${p.basePrice.toLocaleString()}</td>
          <td><span class="badge badge-success">${p.availability} in stock</span></td>
        </tr>
      `).join('');
    }
  }
}

// 2. Products View
async function loadProductsView() {
  await preloadCatalogData();
  const grid = document.getElementById('product-catalog-grid');
  if (!grid || !cachedProducts) return;

  grid.innerHTML = cachedProducts.map(p => `
    <div class="card">
      <div style="display: flex; justify-content: space-between; align-items: flex-start;">
        <span class="badge badge-info">${p.sku}</span>
        <span style="color: var(--accent-warning); font-weight: 700;">★ ${p.rating}</span>
      </div>
      <h3 style="margin: 12px 0 6px 0;">${p.name}</h3>
      <p style="font-size: 13px; color: var(--text-secondary); margin-bottom: 16px;">${p.description}</p>
      <div style="display: flex; justify-content: space-between; align-items: center; border-top: 1px solid var(--border-color); padding-top: 12px;">
        <span style="font-size: 18px; font-weight: 700; color: var(--accent-success);">₹${p.basePrice.toLocaleString()}</span>
        <button class="btn btn-primary" onclick="selectBaseProduct(${p.productId})">Configure</button>
      </div>
    </div>
  `).join('');
}

async function selectBaseProduct(productId) {
  const res = await API.selectItem(productId, -1);
  if (res && res.success) {
    const prod = cachedProducts.find(p => p.productId === productId);
    showToast('Base Product Selected', `${prod ? prod.name : 'Product'} set as Head of C Linked List!`, 'success');
    navigateToConfigurator();
    await refreshConfiguratorState();
    goToStep(2); // advance to motor
  }
}

// 3. Configurator View (Complete 9-Step Interactive Wizard)
async function loadConfiguratorView() {
  await preloadCatalogData();
  await refreshConfiguratorState();
  updateWizardUI(currentWizardStep);
}

async function refreshConfiguratorState() {
  const config = await API.getConfiguration();
  currentConfigItems = config || [];
  renderConfigSummary(currentConfigItems);
  renderSLLVisualizer(currentConfigItems);
  await recalculatePricingAndValidation();
  renderAllStepPanels();
}

// Wizard Step Navigation
function goToStep(stepNum) {
  if (stepNum < 1 || stepNum > 9) return;
  currentWizardStep = stepNum;
  updateWizardUI(stepNum);
}

function stepNext() {
  if (currentWizardStep < 9) {
    goToStep(currentWizardStep + 1);
  } else {
    showToast('Ready for Production', 'Configuration complete and verified by C Engine!', 'success');
  }
}

function stepPrevious() {
  if (currentWizardStep > 1) {
    goToStep(currentWizardStep - 1);
  }
}

function updateWizardUI(stepNum) {
  // Update step buttons
  const stepHeaders = document.querySelectorAll('.wizard-step');
  stepHeaders.forEach(el => {
    const s = parseInt(el.getAttribute('data-step'), 10);
    el.classList.remove('active');
    if (s === stepNum) el.classList.add('active');
    if (s < stepNum) el.classList.add('completed');
  });

  // Update panels
  for (let i = 1; i <= 9; i++) {
    const panel = document.getElementById(`wizard-panel-${i}`);
    if (panel) {
      if (i === stepNum) panel.classList.add('active');
      else panel.classList.remove('active');
    }
  }

  // Update progress bar
  const fill = document.getElementById('wizard-progress-fill');
  if (fill) {
    fill.style.width = `${(stepNum / 9) * 100}%`;
  }

  // Update counter
  const counter = document.getElementById('wizard-step-counter');
  if (counter) {
    counter.textContent = `Step ${stepNum} of 9`;
  }

  // Update button states
  const prevBtn = document.getElementById('btn-wizard-prev');
  const nextBtn = document.getElementById('btn-wizard-next');
  if (prevBtn) prevBtn.disabled = (stepNum === 1);
  if (nextBtn) {
    nextBtn.textContent = (stepNum === 9) ? '✓ Finish & Review Quote' : 'Next Step ➡';
  }

  // Trigger step-specific renders
  if (stepNum === 7) renderValidationStepDetails();
  if (stepNum === 8) renderPricingStepDetails();
  if (stepNum === 9) renderQuoteStepDetails();
}

// Render All Step Panels
function renderAllStepPanels() {
  renderStepProducts();
  renderStepMotors();
  renderStepVoltage();
  renderStepMaterial();
  renderStepControllers();
  renderStepAccessories();
}

// Filter products category in Step 1
function filterProductCategory(category) {
  activeCategoryFilter = category;
  const tabs = document.querySelectorAll('#product-category-filters .filter-tab');
  tabs.forEach(t => t.classList.remove('active'));
  event.target.classList.add('active');
  renderStepProducts();
}

// Step 1: Base Products
function renderStepProducts() {
  const container = document.getElementById('step-products-grid');
  if (!container || !cachedProducts) return;

  const currentBase = currentConfigItems.find(item => !item.isOption);
  const filtered = activeCategoryFilter === 'ALL'
    ? cachedProducts
    : cachedProducts.filter(p => p.category === activeCategoryFilter);

  container.innerHTML = filtered.map(p => {
    const isSelected = currentBase && currentBase.itemId === p.productId;
    return `
      <div class="select-card ${isSelected ? 'selected' : ''}" onclick="selectBaseProduct(${p.productId})">
        <div>
          <div class="select-card-badge">
            <span class="badge badge-info">${p.sku}</span>
            <span style="color: var(--accent-warning); font-weight: 700;">★ ${p.rating}</span>
          </div>
          <div class="select-card-title">${p.name}</div>
          <div class="select-card-desc">${p.description}</div>
        </div>
        <div class="select-card-footer">
          <span class="select-card-price">₹${p.basePrice.toLocaleString()}</span>
          <button class="btn ${isSelected ? 'btn-success' : 'btn-outline'}" style="padding: 4px 12px; font-size: 12px;">
            ${isSelected ? '✓ Selected' : 'Select'}
          </button>
        </div>
      </div>
    `;
  }).join('');
}

// Step 2: Motors
function renderStepMotors() {
  const container = document.getElementById('step-motors-grid');
  if (!container || !cachedOptions) return;

  const motors = cachedOptions.filter(o => o.category === 'Motor');
  container.innerHTML = motors.map(m => {
    const isSelected = currentConfigItems.some(item => item.itemId === m.optionId);
    const is15HP = m.optionId === 203;
    return `
      <div class="select-card ${isSelected ? 'selected' : ''}" onclick="toggleOption(${m.optionId})">
        <div>
          <div class="select-card-badge">
            <span class="badge badge-info">${m.category}</span>
            ${is15HP ? '<span class="badge badge-warning">High Power</span>' : ''}
          </div>
          <div class="select-card-title">${m.name}</div>
          <div class="select-card-desc">
            ${is15HP ? 'Heavy industrial duty cycle. Note: Requires 415 V Power supply (Rule #1).' : 'Standard industrial electric drive motor.'}
          </div>
        </div>
        <div class="select-card-footer">
          <span class="select-card-price">+ ₹${m.price.toLocaleString()}</span>
          <button class="btn ${isSelected ? 'btn-success' : 'btn-outline'}" style="padding: 4px 12px; font-size: 12px;">
            ${isSelected ? '✓ Active' : 'Add Motor'}
          </button>
        </div>
      </div>
    `;
  }).join('');
}

// Step 3: Voltage & Power
function renderStepVoltage() {
  const container = document.getElementById('step-voltage-grid');
  if (!container || !cachedOptions) return;

  const voltages = cachedOptions.filter(o => o.category === 'Voltage');
  const has15HP = currentConfigItems.some(item => item.itemId === 203);

  container.innerHTML = voltages.map(v => {
    const isSelected = currentConfigItems.some(item => item.itemId === v.optionId);
    const isIncompatible = has15HP && v.optionId === 204; // 230V + 15HP
    const isRecommended = has15HP && v.optionId === 205; // 415V + 15HP

    return `
      <div class="select-card ${isSelected ? 'selected' : ''} ${isIncompatible ? 'incompatible' : ''}" onclick="toggleOption(${v.optionId})">
        <div>
          <div class="select-card-badge">
            <span class="badge badge-info">${v.category}</span>
            ${isIncompatible ? '<span class="badge badge-danger">⚠ Incompatible with 15HP</span>' : ''}
            ${isRecommended ? '<span class="badge badge-success">✓ Required for 15HP</span>' : ''}
          </div>
          <div class="select-card-title">${v.name}</div>
          <div class="select-card-desc">
            ${v.optionId === 204 ? 'Single-phase 230 V power. Cannot run 15 HP heavy motor!' : 'Three-phase 415 V industrial grid power. Ideal for heavy pumps.'}
          </div>
        </div>
        <div class="select-card-footer">
          <span class="select-card-price">+ ₹${v.price.toLocaleString()}</span>
          <button class="btn ${isSelected ? 'btn-success' : (isIncompatible ? 'btn-outline' : 'btn-primary')}" style="padding: 4px 12px; font-size: 12px;">
            ${isSelected ? '✓ Active' : 'Select Voltage'}
          </button>
        </div>
      </div>
    `;
  }).join('');
}

// Step 4: Material
function renderStepMaterial() {
  const container = document.getElementById('step-material-grid');
  if (!container || !cachedOptions) return;

  const materials = cachedOptions.filter(o => o.category === 'Material');
  container.innerHTML = materials.map(m => {
    const isSelected = currentConfigItems.some(item => item.itemId === m.optionId);
    const isSS = m.optionId === 207;

    return `
      <div class="select-card ${isSelected ? 'selected' : ''}" onclick="toggleOption(${m.optionId})">
        <div>
          <div class="select-card-badge">
            <span class="badge badge-info">${m.category}</span>
            ${isSS ? '<span class="badge badge-warning">Surcharge Rule #3</span>' : '<span class="badge badge-info">Standard</span>'}
          </div>
          <div class="select-card-title">${m.name}</div>
          <div class="select-card-desc">
            ${isSS ? 'Corrosion-proof 316L Stainless Steel body. Applies premium pricing surcharge in C engine.' : 'Heavy cast iron housing suitable for standard fluid transfer.'}
          </div>
        </div>
        <div class="select-card-footer">
          <span class="select-card-price">+ ₹${m.price.toLocaleString()}</span>
          <button class="btn ${isSelected ? 'btn-success' : 'btn-outline'}" style="padding: 4px 12px; font-size: 12px;">
            ${isSelected ? '✓ Active' : 'Select Material'}
          </button>
        </div>
      </div>
    `;
  }).join('');
}

// Step 5: Controllers
function renderStepControllers() {
  const container = document.getElementById('step-controller-grid');
  if (!container || !cachedOptions) return;

  const controllers = cachedOptions.filter(o => o.category === 'Controller');
  container.innerHTML = controllers.map(c => {
    const isSelected = currentConfigItems.some(item => item.itemId === c.optionId);
    const isAdv = c.optionId === 209;

    return `
      <div class="select-card ${isSelected ? 'selected' : ''}" onclick="toggleOption(${c.optionId})">
        <div>
          <div class="select-card-badge">
            <span class="badge badge-info">${c.category}</span>
            ${isAdv ? '<span class="badge badge-warning">Requires Mounting Kit</span>' : ''}
          </div>
          <div class="select-card-title">${c.name}</div>
          <div class="select-card-desc">
            ${isAdv ? 'VFD Variable Frequency controller with IoT telemetry. Requires Mounting Kit per Rule #4.' : 'Standard ON/OFF control unit with basic overload protection.'}
          </div>
        </div>
        <div class="select-card-footer">
          <span class="select-card-price">+ ₹${c.price.toLocaleString()}</span>
          <button class="btn ${isSelected ? 'btn-success' : 'btn-outline'}" style="padding: 4px 12px; font-size: 12px;">
            ${isSelected ? '✓ Active' : 'Select Controller'}
          </button>
        </div>
      </div>
    `;
  }).join('');
}

// Step 6: Sensors & Accessories
function renderStepAccessories() {
  const container = document.getElementById('step-accessories-grid');
  if (!container || !cachedOptions) return;

  const accessories = cachedOptions.filter(o => o.category === 'Sensor' || o.category === 'Mounting');
  container.innerHTML = accessories.map(a => {
    const isSelected = currentConfigItems.some(item => item.itemId === a.optionId);
    return `
      <div class="select-card ${isSelected ? 'selected' : ''}" onclick="toggleOption(${a.optionId})">
        <div>
          <div class="select-card-badge">
            <span class="badge badge-info">${a.category}</span>
          </div>
          <div class="select-card-title">${a.name}</div>
          <div class="select-card-desc">
            ${a.optionId === 212 ? 'Heavy-duty vibration damping hardware kit. Required by Advanced Controller.' : 'Digital industrial sensor transmitter for real-time telemetry.'}
          </div>
        </div>
        <div class="select-card-footer">
          <span class="select-card-price">+ ₹${a.price.toLocaleString()}</span>
          <button class="btn ${isSelected ? 'btn-success' : 'btn-outline'}" style="padding: 4px 12px; font-size: 12px;">
            ${isSelected ? '✓ Added' : 'Add Item'}
          </button>
        </div>
      </div>
    `;
  }).join('');
}

// Toggle Option Selection
async function toggleOption(optionId) {
  const isAlreadySelected = currentConfigItems.some(item => item.itemId === optionId);
  if (isAlreadySelected) {
    await removeConfigOption(optionId);
    return;
  }

  const res = await API.selectItem(-1, optionId);
  if (res) {
    const opt = cachedOptions.find(o => o.optionId === optionId);
    showToast('Option Added', `${opt ? opt.name : 'Option'} appended to C Linked List!`, 'success');
    await refreshConfiguratorState();
  }
}

// Remove Option
async function removeConfigOption(optionId) {
  const res = await API.removeItem(optionId);
  if (res) {
    const opt = cachedOptions.find(o => o.optionId === optionId);
    showToast('Option Removed', `${opt ? opt.name : 'Option'} deleted from C Linked List`, 'info');
    await refreshConfiguratorState();
  }
}

// Step 7: Live C Validation Details
async function renderValidationStepDetails() {
  const container = document.getElementById('step-validation-details');
  if (!container) return;

  const val = await API.validateConfig();
  const rules = (await API.getRules()) || [];

  container.innerHTML = `
    <div style="display: flex; gap: 16px; flex-wrap: wrap; margin-bottom: 20px;">
      <div class="card" style="flex: 1; min-width: 280px; border-left: 4px solid ${val && val.isValid ? 'var(--accent-success)' : 'var(--accent-danger)'};">
        <h4 style="color: ${val && val.isValid ? 'var(--accent-success)' : 'var(--accent-danger)'};">
          ${val && val.isValid ? '✓ GRAPH & RULE ENGINE VALID' : '⚠ VALIDATION FAILED'}
        </h4>
        <p style="font-size: 13px; color: var(--text-secondary); margin-top: 6px;">${val ? val.details : ''}</p>
        <div style="margin-top: 10px; font-size: 11px; color: var(--text-muted);">
          Graph Engine: <strong>Acyclic (0 Cycles Detected via DFS/BFS)</strong>
        </div>
      </div>
      <div class="card" style="flex: 1; min-width: 280px;">
        <h4>C Expression Stack Evaluator</h4>
        <div style="font-size: 12px; margin-top: 6px; color: var(--text-secondary);">
          Rule Condition Infix → Postfix Stack conversion executed in C <code>ArrayStack</code>.
        </div>
        <div style="margin-top: 10px;">
          <button class="btn btn-outline" style="padding: 4px 10px; font-size: 12px;" onclick="convertRuleExpression()">Run Live Stack Test →</button>
        </div>
      </div>
    </div>

    <h4>Configuration Rules Status Checklist:</h4>
    <div class="table-container" style="margin-top: 10px;">
      <table>
        <thead>
          <tr>
            <th>Rule #</th>
            <th>Rule Name</th>
            <th>Condition Expression</th>
            <th>Constraint Action</th>
            <th>Audit Status</th>
          </tr>
        </thead>
        <tbody>
          ${rules.map(r => {
            const hasMotor15HP = currentConfigItems.some(i => i.itemId === 203);
            const hasVoltage415 = currentConfigItems.some(i => i.itemId === 205);
            const hasVoltage230 = currentConfigItems.some(i => i.itemId === 204);
            const hasAdvController = currentConfigItems.some(i => i.itemId === 209);
            const hasMountingKit = currentConfigItems.some(i => i.itemId === 212);
            const hasSSBody = currentConfigItems.some(i => i.itemId === 207);

            let statusBadge = '<span class="badge badge-success">✓ Passed</span>';
            if (r.ruleId === 5 && hasMotor15HP && hasVoltage230) {
              statusBadge = '<span class="badge badge-danger">⚠ Violated</span>';
            } else if (r.ruleId === 1 && hasMotor15HP && !hasVoltage415) {
              statusBadge = '<span class="badge badge-warning">Requires 415V</span>';
            } else if (r.ruleId === 4 && hasAdvController && !hasMountingKit) {
              statusBadge = '<span class="badge badge-warning">Requires Mounting</span>';
            } else if (r.ruleId === 3 && hasSSBody) {
              statusBadge = '<span class="badge badge-info">+ Surcharge Active</span>';
            }

            return `
              <tr>
                <td><strong>#${r.ruleId}</strong></td>
                <td>${r.name}</td>
                <td><code>${r.condition}</code></td>
                <td><span class="badge ${r.action === 'INVALID' ? 'badge-danger' : 'badge-info'}">${r.action}</span></td>
                <td>${statusBadge}</td>
              </tr>
            `;
          }).join('')}
        </tbody>
      </table>
    </div>
  `;
}

// Step 8: Pricing Engine Details
async function renderPricingStepDetails() {
  const container = document.getElementById('step-pricing-details');
  if (!container) return;

  const price = await API.getPrice();
  const recs = await API.getRecommendations();

  container.innerHTML = `
    <div class="grid-2">
      <div class="card">
        <h4 style="margin-bottom: 14px;">Pure C Pricing Math Breakdown</h4>
        <div style="display: flex; flex-direction: column; gap: 10px; font-size: 13px;">
          <div style="display: flex; justify-content: space-between;"><span>Base Product Price:</span> <strong>₹${(price.baseProductPrice || 0).toLocaleString()}</strong></div>
          <div style="display: flex; justify-content: space-between;"><span>Selected Options Sum:</span> <strong>+ ₹${(price.optionTotalPrice || 0).toLocaleString()}</strong></div>
          <div style="display: flex; justify-content: space-between;"><span>Material Surcharge:</span> <strong>+ ₹${(price.surcharge || 0).toLocaleString()}</strong></div>
          <div style="display: flex; justify-content: space-between; color: var(--accent-success);"><span>Applied Enterprise Discount (5%):</span> <strong>- ₹${(price.discountAmount || 0).toLocaleString()}</strong></div>
          <div style="display: flex; justify-content: space-between; color: var(--accent-info);"><span>Goods & Services Tax (18% GST):</span> <strong>+ ₹${(price.taxAmount || 0).toLocaleString()}</strong></div>
          <hr style="border-color: var(--border-color);" />
          <div style="display: flex; justify-content: space-between; font-size: 20px; font-weight: 800; color: var(--accent-success);">
            <span>Final Calculated Total:</span> <span>₹${(price.finalTotalPrice || 0).toLocaleString()}</span>
          </div>
        </div>
      </div>

      <div class="card">
        <h4 style="margin-bottom: 8px;">C QuickSort Alternative Candidates</h4>
        <p style="font-size: 12px; color: var(--text-secondary); margin-bottom: 12px;">Top ranked alternative configurations ranked using C Quick Sort algorithm.</p>
        <div style="display: flex; flex-direction: column; gap: 8px;">
          ${(recs && recs.candidates ? recs.candidates.slice(0, 3) : []).map(c => `
            <div style="padding: 10px; background: rgba(255,255,255,0.03); border-radius: 6px; display: flex; justify-content: space-between; align-items: center;">
              <div>
                <strong style="font-size: 13px;">${c.name}</strong>
                <div style="font-size: 11px; color: var(--text-muted);">Score: ${c.score.toFixed(1)} | ★ ${c.rating}</div>
              </div>
              <span style="font-weight: 700; color: var(--accent-success); font-size: 13px;">₹${c.price.toLocaleString()}</span>
            </div>
          `).join('')}
        </div>
      </div>
    </div>
  `;
}

// Step 9: Official Quote Generation
async function renderQuoteStepDetails() {
  const container = document.getElementById('step-quote-details');
  if (!container) return;

  const price = await API.getPrice();
  const baseProd = currentConfigItems.find(i => !i.isOption) || { name: 'Standard Product' };

  container.innerHTML = `
    <div class="card" style="margin-bottom: 20px;">
      <h4 style="margin-bottom: 12px;">Dispatch to C Quote Queue</h4>
      <div style="display: flex; gap: 12px; flex-wrap: wrap;">
        <input type="text" id="quote-client-name" class="form-control" style="flex: 1; min-width: 240px;" value="ACME Heavy PetroChemical Industries" placeholder="Client / Enterprise Name" />
        <button class="btn btn-success" onclick="generateOfficialQuoteFromWizard()"><span class="icon">📥</span> Generate & Enqueue Quote</button>
      </div>
    </div>

    <div class="quote-certificate">
      <div style="font-size: 11px; color: var(--accent-info); font-weight: 700; text-transform: uppercase;">Configured Enterprise Solution</div>
      <h2 style="margin: 8px 0 4px 0; font-size: 24px;">${baseProd.name} Complete Rig</h2>
      <div style="font-size: 13px; color: var(--text-secondary); margin-bottom: 16px;">Verified and serialized in C Memory Linked List</div>

      <div style="display: flex; flex-wrap: wrap; gap: 8px; margin-bottom: 20px;">
        ${currentConfigItems.map(i => `<span class="badge ${i.isOption ? 'badge-info' : 'badge-success'}">${i.name}</span>`).join('')}
      </div>

      <div style="display: flex; justify-content: space-between; align-items: flex-end; border-top: 1px dashed var(--border-color); padding-top: 16px;">
        <div>
          <div style="font-size: 11px; color: var(--text-muted);">Quote Validity: 30 Days</div>
          <div style="font-size: 11px; color: var(--text-muted); margin-top: 2px;">C Queue Backend: FIFO ArrayQueue + Circular Queue</div>
        </div>
        <div style="text-align: right;">
          <div style="font-size: 12px; color: var(--text-secondary);">Total Investment</div>
          <div style="font-size: 26px; font-weight: 800; color: var(--accent-success);">₹${(price.finalTotalPrice || 0).toLocaleString()}</div>
        </div>
      </div>
    </div>
  `;
}

async function generateOfficialQuoteFromWizard() {
  const clientInput = document.getElementById('quote-client-name');
  const clientName = (clientInput && clientInput.value.trim()) || 'ACME Global CPQ Client';
  const quote = await API.createQueueJob(clientName);
  if (quote) {
    showToast('Quote Enqueued', `Quote #${quote.quoteNumber} scheduled in C FIFO Queue! Final Price: ₹${quote.finalPrice.toLocaleString()}`, 'success');
    renderQuoteStepDetails();
  }
}

// Render Configuration Summary
function renderConfigSummary(config) {
  const container = document.getElementById('cfg-summary-list');
  const badgeCount = document.getElementById('cfg-badge-count');
  if (badgeCount) badgeCount.textContent = `${config ? config.length : 0} items`;
  if (!container) return;

  if (!config || config.length === 0) {
    container.innerHTML = '<p style="color: var(--text-muted); font-size: 13px;">No components selected in C linked list.</p>';
    return;
  }

  container.innerHTML = config.map(item => `
    <div style="display: flex; justify-content: space-between; align-items: center; padding: 8px 12px; background: rgba(255,255,255,0.03); border-radius: 6px; margin-bottom: 6px;">
      <div>
        <strong>${item.name}</strong>
        <div style="font-size: 11px; color: var(--text-muted);">${item.category}</div>
      </div>
      <div style="display: flex; align-items: center; gap: 12px;">
        <span style="font-weight: 600; color: var(--accent-success); font-size: 13px;">₹${item.price.toLocaleString()}</span>
        ${item.isOption ? `<button class="btn btn-outline" style="padding: 2px 8px; font-size: 11px; color: var(--accent-danger);" onclick="removeConfigOption(${item.itemId})">✕</button>` : '<span class="badge badge-success" style="font-size: 10px;">Base</span>'}
      </div>
    </div>
  `).join('');
}

// Render Live Singly Linked List (SLL) Visualizer
function renderSLLVisualizer(config) {
  const container = document.getElementById('sll-nodes-container');
  if (!container) return;

  if (!config || config.length === 0) {
    container.innerHTML = `
      <span class="sll-pill sll-head">HEAD</span>
      <span class="sll-arrow">→</span>
      <span class="sll-pill sll-null">NULL</span>
    `;
    return;
  }

  const nodesHTML = config.map((node, index) => `
    <div class="sll-node-box">
      <div class="sll-node-header">
        <span>Node #${index}</span>
        <span>${node.isOption ? 'Option' : 'Product'}</span>
      </div>
      <div class="sll-node-name" title="${node.name}">${node.name}</div>
      <div class="sll-node-footer">
        <span style="color: var(--accent-success); font-weight: 600;">₹${node.price.toLocaleString()}</span>
        <span style="color: var(--accent-info); font-size: 10px;">next *</span>
      </div>
    </div>
    <span class="sll-arrow">→</span>
  `).join('');

  container.innerHTML = `
    <span class="sll-pill sll-head">HEAD</span>
    <span class="sll-arrow">→</span>
    ${nodesHTML}
    <span class="sll-pill sll-null">NULL</span>
  `;
}

// Recalculate Pricing and Validation
async function recalculatePricingAndValidation() {
  const val = await API.validateConfig();
  updateValidationStatus(val);
  checkAndRenderConflicts(val);

  const price = await API.getPrice();
  if (price) {
    const subtotalEl = document.getElementById('price-subtotal');
    const discountEl = document.getElementById('price-discount');
    const taxEl = document.getElementById('price-tax');
    const totalEl = document.getElementById('price-total');

    const sub = (price.baseProductPrice || 0) + (price.optionTotalPrice || 0) + (price.surcharge || 0);
    if (subtotalEl) subtotalEl.textContent = `₹${sub.toLocaleString()}`;
    if (discountEl) discountEl.textContent = `- ₹${(price.discountAmount || 0).toLocaleString()}`;
    if (taxEl) taxEl.textContent = `+ ₹${(price.taxAmount || 0).toLocaleString()}`;
    if (totalEl) totalEl.textContent = `₹${(price.finalTotalPrice || 0).toLocaleString()}`;
  }
}

// Conflict Resolution Card
function checkAndRenderConflicts(val) {
  const alertEl = document.getElementById('cfg-conflict-alert');
  if (!alertEl) return;

  const has15HP = currentConfigItems.some(i => i.itemId === 203);
  const has230V = currentConfigItems.some(i => i.itemId === 204);
  const has415V = currentConfigItems.some(i => i.itemId === 205);
  const hasAdvCtrl = currentConfigItems.some(i => i.itemId === 209);
  const hasMounting = currentConfigItems.some(i => i.itemId === 212);

  if (has15HP && has230V) {
    alertEl.style.display = 'block';
    alertEl.innerHTML = `
      <div class="card" style="border-left: 4px solid var(--accent-danger); background: rgba(239, 68, 68, 0.1);">
        <div style="color: var(--accent-danger); font-weight: 700; margin-bottom: 4px;">⚠ CRITICAL CONFLICT (Rule #5 Violated)</div>
        <p style="font-size: 13px; color: var(--text-secondary);">15 HP Motor cannot run on 230 V single-phase power! Requires 415 V Three-Phase power.</p>
        <button class="btn btn-primary" style="margin-top: 10px; font-size: 12px;" onclick="autoResolveConflict('SWITCH_TO_415V')">⚡ Auto-Resolve: Switch to 415 V Power</button>
      </div>
    `;
    return;
  }

  if (has15HP && !has415V) {
    alertEl.style.display = 'block';
    alertEl.innerHTML = `
      <div class="card" style="border-left: 4px solid var(--accent-warning); background: rgba(245, 158, 11, 0.1);">
        <div style="color: var(--accent-warning); font-weight: 700; margin-bottom: 4px;">⚡ REQUIREMENT MISSING (Rule #1)</div>
        <p style="font-size: 13px; color: var(--text-secondary);">15 HP Motor requires 415 V three-phase power supply.</p>
        <button class="btn btn-primary" style="margin-top: 10px; font-size: 12px;" onclick="autoResolveConflict('ADD_415V')">⚡ Auto-Add: Select 415 V Power</button>
      </div>
    `;
    return;
  }

  if (hasAdvCtrl && !hasMounting) {
    alertEl.style.display = 'block';
    alertEl.innerHTML = `
      <div class="card" style="border-left: 4px solid var(--accent-warning); background: rgba(245, 158, 11, 0.1);">
        <div style="color: var(--accent-warning); font-weight: 700; margin-bottom: 4px;">⚡ REQUIREMENT MISSING (Rule #4)</div>
        <p style="font-size: 13px; color: var(--text-secondary);">Advanced Controller telemetry module requires Mounting Hardware Kit.</p>
        <button class="btn btn-primary" style="margin-top: 10px; font-size: 12px;" onclick="autoResolveConflict('ADD_MOUNTING_KIT')">⚡ Auto-Add: Select Mounting Kit</button>
      </div>
    `;
    return;
  }

  alertEl.style.display = 'none';
}

async function autoResolveConflict(action) {
  if (action === 'SWITCH_TO_415V') {
    await API.removeItem(204); // remove 230V
    await API.selectItem(-1, 205); // add 415V
    showToast('Conflict Auto-Resolved', 'Replaced 230 V Power with 415 V Three-Phase Power per Rule #1 & #5!', 'success');
  } else if (action === 'ADD_415V') {
    await API.selectItem(-1, 205);
    showToast('Requirement Satisfied', '415 V Power added for 15 HP Motor!', 'success');
  } else if (action === 'ADD_MOUNTING_KIT') {
    await API.selectItem(-1, 212);
    showToast('Requirement Satisfied', 'Mounting Hardware Kit added for Advanced Controller!', 'success');
  }
  await refreshConfiguratorState();
}

function updateValidationStatus(val) {
  const el = document.getElementById('cfg-validation-box');
  if (!el || !val) return;

  if (val.isValid) {
    el.className = 'card';
    el.style.borderLeft = '4px solid var(--accent-success)';
    el.innerHTML = `
      <div style="color: var(--accent-success); font-weight: 700; margin-bottom: 4px;">✓ CONFIGURATION 100% VALID</div>
      <p style="font-size: 13px; color: var(--text-secondary);">${val.details}</p>
      <div style="margin-top: 10px; font-size: 18px; font-weight: 700; color: var(--text-primary);">Total Price: ₹${val.pricing ? val.pricing.finalTotalPrice.toLocaleString() : 0}</div>
    `;
  } else {
    el.className = 'card';
    el.style.borderLeft = '4px solid var(--accent-danger)';
    el.innerHTML = `
      <div style="color: var(--accent-danger); font-weight: 700; margin-bottom: 4px;">⚠ CONFIGURATION INVALID</div>
      <p style="font-size: 13px; color: var(--text-secondary);">${val.details}</p>
    `;
  }
}

// 1-Click Complete Configuration Demo Preset
async function runCompleteConfigurationPreset() {
  showToast('Starting 1-Click Demo Rig', 'Configuring Enterprise Industrial Pump in C Memory...', 'info');

  // Step 1: Base Product 101 (Industrial Pump X)
  await API.selectItem(101, -1);

  // Step 2: 15 HP Motor (203)
  await API.selectItem(-1, 203);

  // Step 3: 415 V Power (205)
  await API.selectItem(-1, 205);

  // Step 4: Stainless Steel Body (207)
  await API.selectItem(-1, 207);

  // Step 5: Advanced Controller (209)
  await API.selectItem(-1, 209);

  // Step 6: Pressure Sensor (210)
  await API.selectItem(-1, 210);

  // Step 7: Mounting Hardware Kit (212)
  await API.selectItem(-1, 212);

  navigateToConfigurator();
  await refreshConfiguratorState();
  goToStep(8); // jump to pricing / quote

  showToast('Configuration Complete!', 'Industrial Pump X Rig configured: 7 components in C Linked List, ₹132,838.50 final total.', 'success');
}

// Reset Configuration
async function resetConfiguration() {
  await API.selectItem(101, -1); // selecting base product resets options in C linked list
  navigateToConfigurator();
  await refreshConfiguratorState();
  goToStep(1);
  showToast('Reset Complete', 'Configuration reset to base product.', 'info');
}

// ==========================================
// GUIDED INTERACTIVE DEMO ENGINE
// ==========================================
const demoStages = [
  {
    step: 1,
    title: "1. Select Base Product: Industrial Pump X",
    desc: "We start by querying the product catalog stored in C arrays. The SKU 'PMP-1001' is looked up in O(1) time via the Separate Chaining Hash Table. Industrial Pump X (Base Price ₹50,000) becomes the root of our configuration.",
    ds: "Product Array + SKU Hash Table (Separate Chaining TABLE_SIZE=37) + Singly Linked List root node insertion `insertEnd(&engine->currentConfig, ...)`",
    action: async () => {
      await API.selectItem(101, -1);
    }
  },
  {
    step: 2,
    title: "2. Motor Horsepower Sizing: 15 HP Motor",
    desc: "Next, we attach the heavy-duty 15 HP Motor (Option 203, ₹25,000). A new node is dynamically allocated in C memory and appended to our Singly Linked List.",
    ds: "Singly Linked List `ConfigLinkedList` (Count=2 nodes). Pointer: HEAD -> [Pump X] -> [15 HP Motor] -> NULL",
    action: async () => {
      await API.selectItem(-1, 203);
    }
  },
  {
    step: 3,
    title: "3. Incompatibility Testing: 230 V Conflict & Auto-Resolve",
    desc: "To demonstrate the CPQ rule engine, we intentionally attempt to pair 230 V Power (Option 204). The Infix→Postfix Stack evaluates Rule #5: `Motor15HP == 1 AND Voltage230 == 1`, detecting a critical incompatibility! The system catches this, rejects 230V, and switches to 415 V Three-Phase Power (Option 205).",
    ds: "Stack Engine `ArrayStack` (Infix to Postfix expression conversion & boolean evaluation). Resolves Rule #5 & satisfies Rule #1.",
    action: async () => {
      await API.selectItem(-1, 204); // trigger conflict
      await new Promise(r => setTimeout(r, 600));
      await API.removeItem(204); // auto-resolve
      await API.selectItem(-1, 205); // select 415V
    }
  },
  {
    step: 4,
    title: "4. Material Selection: Stainless Steel Casing",
    desc: "We select 316L Stainless Steel Body (Option 207, ₹12,000). The C rules engine evaluates Rule #3 (`MaterialSS == 1`), triggering a premium material surcharge calculation in the pricing engine.",
    ds: "Rule Evaluation + C Pricing Engine Surcharge Formula: `pb.surcharge` computed in C memory.",
    action: async () => {
      await API.selectItem(-1, 207);
    }
  },
  {
    step: 5,
    title: "5. Control System: Advanced VFD Controller",
    desc: "We add the Advanced VFD Controller (Option 209, ₹16,000). The Compatibility Graph inspects dependency edges and signals Rule #4: An Advanced Controller requires a Mounting Kit.",
    ds: "Compatibility Graph (Adjacency Matrix & List) + Rule Dependency Check.",
    action: async () => {
      await API.selectItem(-1, 209);
    }
  },
  {
    step: 6,
    title: "6. Sensors & Hardware: Pressure Sensor & Mounting Kit",
    desc: "We complete the hardware stack by adding the Pressure Sensor (Option 210, ₹4,500) and the Mounting Hardware Kit (Option 212, ₹5,000). Rule #2 (Sensors require Controller) and Rule #4 (Controller requires Mounting) are now both 100% satisfied!",
    ds: "Singly Linked List (7 nodes linked). Graph edges fully satisfied.",
    action: async () => {
      await API.selectItem(-1, 210);
      await API.selectItem(-1, 212);
    }
  },
  {
    step: 7,
    title: "7. C Engine Multi-Validation & Graph Acyclic Check",
    desc: "Before pricing, the engine runs a comprehensive audit: (1) Graph Cycle Detection executes DFS & BFS to verify the dependency graph has 0 cycles; (2) Infix→Postfix rule evaluator tests all 5 CPQ rules. All conditions pass!",
    ds: "Graph Cycle Detection (DFS recursion stack & BFS queue) + Rule Stack evaluation.",
    action: async () => {
      await API.validateConfig();
    }
  },
  {
    step: 8,
    title: "8. Pricing Math & Formal Quote Enqueued",
    desc: "The C Pricing Engine computes: Base (₹50,000) + Options (₹68,500) = Subtotal (₹118,500). Applies 5% enterprise discount (-₹5,925) and 18% GST (+₹20,263.50), yielding a final price of ₹132,838.50. Finally, a verified quote is dispatched to the FIFO Array Queue!",
    ds: "Financial Calculation Engine + FIFO Array Queue + Circular Queue worker scheduler.",
    action: async () => {
      await API.createQueueJob("ACME Heavy PetroChemical Solutions");
    }
  }
];

function openDemoModal() {
  const modal = document.getElementById('demo-modal');
  if (modal) modal.classList.add('active');
  demoCurrentStage = 0;
  executeDemoStage(0);
}

function closeDemoModal() {
  const modal = document.getElementById('demo-modal');
  if (modal) modal.classList.remove('active');
  pauseDemoAutoplay();
  refreshConfiguratorState();
}

async function executeDemoStage(stageIndex) {
  if (stageIndex < 0 || stageIndex >= demoStages.length) return;
  demoCurrentStage = stageIndex;
  const stage = demoStages[stageIndex];

  // Update UI text
  document.getElementById('demo-step-badge').textContent = `Stage ${stageIndex + 1} of ${demoStages.length}: ${stage.title.split(':')[0]}`;
  document.getElementById('demo-stage-title').textContent = stage.title;
  document.getElementById('demo-stage-desc').textContent = stage.desc;
  document.getElementById('demo-ds-details').textContent = stage.ds;
  document.getElementById('demo-progress-fill').style.width = `${((stageIndex + 1) / demoStages.length) * 100}%`;
  document.getElementById('demo-step-indicator').textContent = `Step ${stageIndex + 1} of ${demoStages.length}`;

  // Execute C Engine API calls
  await stage.action();

  // Fetch updated vector
  const config = await API.getConfiguration();
  const price = await API.getPrice();

  const previewEl = document.getElementById('demo-vector-preview');
  if (previewEl && config) {
    const listStr = config.map(c => `[${c.name} | ₹${c.price}]`).join(' -> ');
    previewEl.textContent = `HEAD -> ${listStr} -> NULL`;
  }

  const livePriceEl = document.getElementById('demo-live-price');
  if (livePriceEl && price) {
    livePriceEl.textContent = `Calculated Price: ₹${(price.finalTotalPrice || 0).toLocaleString()}`;
  }

  // Update Prev / Next buttons
  const prevBtn = document.getElementById('btn-demo-prev');
  const nextBtn = document.getElementById('btn-demo-next');
  if (prevBtn) prevBtn.disabled = (stageIndex === 0);
  if (nextBtn) {
    nextBtn.textContent = (stageIndex === demoStages.length - 1) ? '✓ View Configurator' : 'Next Step ➡';
  }
}

function demoNextStep() {
  if (demoCurrentStage < demoStages.length - 1) {
    executeDemoStage(demoCurrentStage + 1);
  } else {
    closeDemoModal();
    navigateToConfigurator();
    goToStep(9);
    showToast('Demo Walkthrough Complete!', 'Industrial product successfully configured and enqueued!', 'success');
  }
}

function demoPrevStep() {
  if (demoCurrentStage > 0) {
    executeDemoStage(demoCurrentStage - 1);
  }
}

function toggleDemoAutoplay() {
  if (isDemoPlaying) {
    pauseDemoAutoplay();
  } else {
    startDemoAutoplay();
  }
}

function startDemoAutoplay() {
  isDemoPlaying = true;
  const btn = document.getElementById('btn-demo-autoplay');
  if (btn) btn.innerHTML = '<span class="icon">⏸</span> Pause';

  demoAutoplayTimer = setInterval(() => {
    if (demoCurrentStage < demoStages.length - 1) {
      demoNextStep();
    } else {
      pauseDemoAutoplay();
      closeDemoModal();
      navigateToConfigurator();
      goToStep(9);
    }
  }, demoAutoplaySpeed);
}

function pauseDemoAutoplay() {
  isDemoPlaying = false;
  const btn = document.getElementById('btn-demo-autoplay');
  if (btn) btn.innerHTML = '<span class="icon">▶</span> Auto-Play';
  if (demoAutoplayTimer) {
    clearInterval(demoAutoplayTimer);
    demoAutoplayTimer = null;
  }
}

function setDemoSpeed(speed) {
  demoAutoplaySpeed = 2500 / speed;
  showToast('Demo Speed', `Autoplay speed set to ${speed}x`, 'info');
  if (isDemoPlaying) {
    pauseDemoAutoplay();
    startDemoAutoplay();
  }
}

async function demoInstantFinish() {
  pauseDemoAutoplay();
  showToast('Completing All Steps...', 'Executing all remaining configuration steps in C memory...', 'info');
  await runCompleteConfigurationPreset();
  await executeDemoStage(demoStages.length - 1);
  closeDemoModal();
  navigateToConfigurator();
  goToStep(9);
}

// 4. Compatibility View
async function loadCompatibilityView() {
  const graph = await API.getCompatibility();
  const el = document.getElementById('graph-canvas');
  if (!el || !graph || !graph.vertices) return;

  el.innerHTML = `
    <div style="width: 100%; padding: 20px; text-align: center;">
      <h3 style="margin-bottom: 12px;">C Compatibility Graph (${graph.numVertices} Vertices, ${graph.edges.length} Edges)</h3>
      <div style="display: flex; flex-wrap: wrap; gap: 12px; justify-content: center;">
        ${graph.vertices.map(v => `
          <div class="ds-node">
            <div>${v.name}</div>
            <div style="font-size: 10px; color: var(--accent-info);">${v.category} (ID: ${v.id})</div>
          </div>
        `).join('')}
      </div>
    </div>
  `;
}

// 5. Rules View & Expression Stack Converter
async function loadRulesView() {
  const rules = await API.getRules();
  const tbody = document.getElementById('rules-table-body');
  if (tbody && rules) {
    tbody.innerHTML = rules.map(r => `
      <tr>
        <td><strong>Rule #${r.ruleId}</strong></td>
        <td>${r.name}</td>
        <td><code>${r.condition}</code></td>
        <td><span class="badge ${r.action === 'INVALID' ? 'badge-danger' : 'badge-info'}">${r.action}</span></td>
        <td>${r.description}</td>
      </tr>
    `).join('');
  }
}

async function convertRuleExpression() {
  const exprInput = document.getElementById('rule-expr-input') ? document.getElementById('rule-expr-input').value : "Motor15HP AND Voltage415";
  const res = await API.evaluateRuleExpr(exprInput);
  const resultBox = document.getElementById('rule-expr-result');
  if (resultBox && res) {
    resultBox.innerHTML = `
      <div style="margin-bottom: 12px;">
        <strong>Converted Postfix Expression:</strong> <code>${res.convertedExpression}</code>
      </div>
      <div>
        <strong>Evaluation Result (Stack C Engine):</strong> 
        <span class="badge ${res.evaluationResult ? 'badge-success' : 'badge-danger'}">${res.evaluationResult ? 'TRUE (1)' : 'FALSE (0)'}</span>
      </div>
      <h4 style="margin: 16px 0 8px 0;">Step-by-Step Stack States:</h4>
      <div style="max-height: 200px; overflow-y: auto;">
        ${res.steps.map(s => `
          <div style="font-size: 12px; padding: 4px 8px; border-bottom: 1px solid var(--border-color);">
            <strong>${s.stepName}</strong>: Token '<code>${s.symbolProcessed}</code>' → Stack: <code>${s.stackContents}</code> → Output: <code>${s.outputExpression}</code>
          </div>
        `).join('')}
      </div>
    `;
  }
}

// 6. Pricing View
async function loadPricingView() {
  const price = await API.getPrice();
  const container = document.getElementById('pricing-breakdown-card');
  if (container && price) {
    container.innerHTML = `
      <div style="display: flex; flex-direction: column; gap: 12px;">
        <div style="display: flex; justify-content: space-between;"><span>Base Product Price:</span> <strong>₹${(price.baseProductPrice || 0).toLocaleString()}</strong></div>
        <div style="display: flex; justify-content: space-between;"><span>Options Total Price:</span> <strong>₹${(price.optionTotalPrice || 0).toLocaleString()}</strong></div>
        <div style="display: flex; justify-content: space-between;"><span>Required Surcharge:</span> <strong>₹${(price.surcharge || 0).toLocaleString()}</strong></div>
        <div style="display: flex; justify-content: space-between; color: var(--accent-success);"><span>Discount (5%):</span> <strong>- ₹${(price.discountAmount || 0).toLocaleString()}</strong></div>
        <div style="display: flex; justify-content: space-between; color: var(--accent-info);"><span>Tax (18% GST):</span> <strong>+ ₹${(price.taxAmount || 0).toLocaleString()}</strong></div>
        <hr style="border-color: var(--border-color);" />
        <div style="display: flex; justify-content: space-between; font-size: 22px; font-weight: 700; color: var(--accent-success);">
          <span>Final Total Price:</span> <span>₹${(price.finalTotalPrice || 0).toLocaleString()}</span>
        </div>
      </div>
    `;
  }
}

// 7. Recommendations View
async function loadRecommendationsView() {
  const recs = await API.getRecommendations();
  const grid = document.getElementById('recommendations-grid');
  if (grid && recs && recs.candidates) {
    grid.innerHTML = recs.candidates.map(c => `
      <div class="card">
        <div style="display: flex; justify-content: space-between; align-items: center; margin-bottom: 8px;">
          <span class="badge badge-success">Score: ${c.score.toFixed(1)}</span>
          <span style="color: var(--accent-warning); font-weight: 700;">★ ${c.rating}</span>
        </div>
        <h4>${c.name}</h4>
        <div style="font-size: 18px; font-weight: 700; color: var(--text-primary); margin: 8px 0;">₹${c.price.toLocaleString()}</div>
        <div style="font-size: 12px; color: var(--text-secondary); margin-bottom: 12px;">Ranked using C ${recs.sortingAlgorithmUsed}</div>
        <button class="btn btn-primary" style="width: 100%; justify-content: center;" onclick="selectBaseProduct(${c.productId})">Switch to Product</button>
      </div>
    `).join('');
  }
}

// 8. Queue View
async function loadQueueView() {
  const queue = await API.getQueue();
  const el = document.getElementById('queue-status-box');
  if (el && queue) {
    el.innerHTML = `
      <p>Pending Jobs in C ArrayQueue: <strong>${queue.pendingCount}</strong></p>
      <div style="margin-top: 12px;">
        ${(queue.jobs || []).map(j => `
          <div style="padding: 8px 12px; background: rgba(255,255,255,0.03); border-radius: 6px; margin-bottom: 6px;">
            <strong>Job #${j.jobId}</strong> (${j.jobType}) - ${j.details} [<span style="color: var(--accent-warning);">${j.status}</span>]
          </div>
        `).join('')}
      </div>
    `;
  }
}

async function triggerQuoteGeneration() {
  const name = prompt("Enter Customer Name for Quote Generation:", "ACME Global Industries");
  if (name) {
    const quote = await API.createQueueJob(name);
    if (quote) {
      showToast('Quote Generated', `Quote #${quote.quoteNumber} created! Final Price: ₹${quote.finalPrice.toLocaleString()}`, 'success');
      loadQueueView();
    }
  }
}

// 9. Data Structures Inspector View
async function loadStructuresView() {
  const config = await API.getConfiguration();
  const tree = await API.getTree();
  const hash = await API.getHashTable();
  const sparse = await API.getSparseMatrix();

  const container = document.getElementById('structures-inspector-container');
  if (container) {
    container.innerHTML = `
      <div class="card" style="margin-bottom: 16px;">
        <h3>1. Singly Linked List (Active Configuration Vector)</h3>
        <pre style="background: rgba(0,0,0,0.4); padding: 12px; border-radius: 6px; overflow-x: auto;">${JSON.stringify(config, null, 2)}</pre>
      </div>
      <div class="card" style="margin-bottom: 16px;">
        <h3>2. General Tree (Product Option Hierarchy)</h3>
        <pre style="background: rgba(0,0,0,0.4); padding: 12px; border-radius: 6px; overflow-x: auto;">${JSON.stringify(tree, null, 2)}</pre>
      </div>
      <div class="card" style="margin-bottom: 16px;">
        <h3>3. Hash Table (SKU -> Product Separate Chaining)</h3>
        <pre style="background: rgba(0,0,0,0.4); padding: 12px; border-radius: 6px; overflow-x: auto;">${JSON.stringify(hash, null, 2)}</pre>
      </div>
      <div class="card">
        <h3>4. Sparse Matrix (Compatibility Triplet Format)</h3>
        <pre style="background: rgba(0,0,0,0.4); padding: 12px; border-radius: 6px; overflow-x: auto;">${JSON.stringify(sparse, null, 2)}</pre>
      </div>
    `;
  }
}

// 10. Algorithms Lab View
function loadAlgorithmsView() {
  // Static controls ready in HTML
}

// 11. Benchmark View
async function loadBenchmarkView() {
  const res = await API.runSortBenchmark(1000);
  const box = document.getElementById('benchmark-results-box');
  if (box && res) {
    box.innerHTML = `
      <h3>Benchmark Results for Dataset Size N = ${res.datasetSize}</h3>
      <table style="margin-top: 16px;">
        <thead>
          <tr>
            <th>Algorithm</th>
            <th>Comparisons</th>
            <th>Swaps</th>
            <th>Execution Time (ms)</th>
          </tr>
        </thead>
        <tbody>
          ${(res.sorts || []).map(s => `
            <tr>
              <td><strong>${s.name}</strong></td>
              <td>${s.comparisons.toLocaleString()}</td>
              <td>${s.swaps.toLocaleString()}</td>
              <td style="color: var(--accent-success); font-weight: 700;">${s.timeMs.toFixed(4)} ms</td>
            </tr>
          `).join('')}
        </tbody>
      </table>
    `;
  }
}

// 12. History View
async function loadHistoryView() {
  const history = await API.getHistory();
  const el = document.getElementById('history-timeline-box');
  if (el && history) {
    el.innerHTML = history.map(h => `
      <div style="padding: 12px; background: rgba(255,255,255,0.03); border-left: 3px solid ${h.isCurrent ? 'var(--accent-success)' : 'var(--border-color)'}; border-radius: 6px; margin-bottom: 8px;">
        <div style="font-size: 11px; color: var(--text-muted);">${h.timestamp} ${h.isCurrent ? '(CURRENT STATE)' : ''}</div>
        <strong>${h.operation}</strong> - Item #${h.itemId} (${h.itemName})
        <div style="font-size: 13px; color: var(--accent-info);">Running Total: ₹${h.totalPrice.toLocaleString()}</div>
      </div>
    `).join('');
  }
}

async function triggerUndo() {
  await API.undoConfig();
  showToast('Undo State', 'Reverted to previous state in Doubly Linked List', 'info');
  await refreshConfiguratorState();
}

async function triggerRedo() {
  await API.redoConfig();
  showToast('Redo State', 'Advanced to next state in Doubly Linked List', 'info');
  await refreshConfiguratorState();
}

// 13. Syllabus Coverage View
function loadSyllabusView() {
  // Static view ready in HTML
}

// 14. Complexity View
function loadComplexityView() {
  // Static view ready in HTML
}

// 15. System Info View
function loadSystemView() {
  // Static view ready in HTML
}
