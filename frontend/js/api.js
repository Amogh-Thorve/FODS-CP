// CONFIGCORE - API Client
const API_BASE = '';

async function apiRequest(endpoint, method = 'GET', body = null) {
  try {
    const options = {
      method: method,
      headers: { 'Content-Type': 'application/json' }
    };
    if (body) {
      options.body = JSON.stringify(body);
    }
    const res = await fetch(`${API_BASE}${endpoint}`, options);
    if (!res.ok) throw new Error(`HTTP ${res.status}`);
    return await res.json();
  } catch (err) {
    console.error(`API Error on ${endpoint}:`, err);
    return null;
  }
}

// Product APIs
const API = {
  getProducts: () => apiRequest('/api/products'),
  getOptions: () => apiRequest('/api/options'),
  getConfiguration: () => apiRequest('/api/configuration'),
  selectItem: (productId, optionId) => apiRequest('/api/configuration/select', 'POST', { productId, optionId }),
  removeItem: (optionId) => apiRequest('/api/configuration/remove', 'POST', { optionId }),
  undoConfig: () => apiRequest('/api/configuration/undo', 'POST'),
  redoConfig: () => apiRequest('/api/configuration/redo', 'POST'),
  validateConfig: () => apiRequest('/api/configuration/validate', 'POST'),
  getPrice: () => apiRequest('/api/configuration/price'),
  getRecommendations: () => apiRequest('/api/recommendations'),
  getCompatibility: () => apiRequest('/api/compatibility'),
  getRules: () => apiRequest('/api/rules'),
  evaluateRuleExpr: (expression) => apiRequest('/api/rules/evaluate', 'POST', { expression }),
  getHistory: () => apiRequest('/api/history'),
  getQueue: () => apiRequest('/api/queue'),
  createQueueJob: (customerName) => apiRequest('/api/queue', 'POST', { customerName }),
  getTree: () => apiRequest('/api/tree'),
  getSparseMatrix: () => apiRequest('/api/sparse-matrix'),
  getHashTable: () => apiRequest('/api/hash-table'),
  getJosephus: (n, k) => apiRequest('/api/josephus', 'GET'),
  runSortBenchmark: (size) => apiRequest('/api/benchmark/sort', 'POST', { size })
};
