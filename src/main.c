#include <stdio.h>
#include <stdlib.h>
#include "configuration.h"
#include "quote.h"
#include "api.h"
#include "server.h"

int main(int argc, char *argv[]) {
    int port = 8080;
    if (argc > 1) {
        port = atoi(argv[1]);
    }

    printf("=======================================================\n");
    printf("     CONFIGCORE - Product Configuration Engine        \n");
    printf("=======================================================\n");

    CPQEngine engine;
    initCPQEngine(&engine);

    // Initialize Products
    addProduct(&engine.products, (Product){101, "PMP-1001", "Industrial Pump X", "Industrial Pumps", "High-efficiency centrifugal pump", 50000.0, 15, 4.8, 1});
    addProduct(&engine.products, (Product){102, "PMP-1002", "Industrial Pump Y", "Industrial Pumps", "Positive displacement pump", 62000.0, 8, 4.6, 2});
    addProduct(&engine.products, (Product){103, "MTR-2001", "Industrial Motor A", "Electric Motors", "Three-phase induction motor", 25000.0, 25, 4.9, 1});
    addProduct(&engine.products, (Product){104, "MTR-2002", "Industrial Motor B", "Electric Motors", "Explosion-proof heavy duty motor", 38000.0, 12, 4.7, 2});
    addProduct(&engine.products, (Product){105, "CTL-3001", "Control Unit A", "Controllers", "Programmable logic controller", 18000.0, 30, 4.5, 1});
    addProduct(&engine.products, (Product){106, "CTL-3002", "Control Unit B", "Controllers", "Advanced VFD speed controller", 29000.0, 18, 4.8, 2});
    addProduct(&engine.products, (Product){107, "SNS-4001", "Pressure Sensor", "Sensors", "Piezoelectric pressure transmitter", 4500.0, 50, 4.6, 1});
    addProduct(&engine.products, (Product){108, "SNS-4002", "Temperature Sensor", "Sensors", "RTD PT100 temperature probe", 3200.0, 45, 4.4, 2});
    addProduct(&engine.products, (Product){109, "PWR-5001", "Power Module", "Power Supplies", "DIN-rail 24V DC regulated supply", 7500.0, 40, 4.7, 1});
    addProduct(&engine.products, (Product){110, "MNT-6001", "Mounting Kit", "Hardware", "Heavy-duty vibration damping kit", 5000.0, 60, 4.3, 1});

    // Initialize Options
    addOption(&engine.options, (Option){201, "5 HP Motor", "Motor", 12000.0, 103});
    addOption(&engine.options, (Option){202, "10 HP Motor", "Motor", 18000.0, 103});
    addOption(&engine.options, (Option){203, "15 HP Motor", "Motor", 25000.0, 104});
    addOption(&engine.options, (Option){204, "230 V Power", "Voltage", 3000.0, 109});
    addOption(&engine.options, (Option){205, "415 V Power", "Voltage", 6000.0, 109});
    addOption(&engine.options, (Option){206, "Cast Iron Body", "Material", 4000.0, 101});
    addOption(&engine.options, (Option){207, "Stainless Steel Body", "Material", 12000.0, 101});
    addOption(&engine.options, (Option){208, "Basic Controller", "Controller", 8000.0, 105});
    addOption(&engine.options, (Option){209, "Advanced Controller", "Controller", 16000.0, 106});
    addOption(&engine.options, (Option){210, "Pressure Sensor Option", "Sensor", 4500.0, 107});
    addOption(&engine.options, (Option){211, "Temperature Sensor Option", "Sensor", 3200.0, 108});
    addOption(&engine.options, (Option){212, "Mounting Hardware Kit", "Mounting", 5000.0, 110});

    // Initialize Rules
    addRule(&engine.ruleSet, (Rule){1, "High Power Voltage Rule", "Motor15HP == 1 AND Voltage415 == 0", "REQUIRE", "415 V Power", "If 15 HP Motor is selected, then 415 V Power must be selected."});
    addRule(&engine.ruleSet, (Rule){2, "Sensor Controller Dependency", "SensorSelected == 1 AND ControllerSelected == 0", "REQUIRE", "Advanced Controller", "If Sensor is selected, then a Controller is required."});
    addRule(&engine.ruleSet, (Rule){3, "Stainless Steel Surcharge", "MaterialSS == 1", "SURCHARGE", "Premium Tax", "If Stainless Steel is chosen, premium pricing applies."});
    addRule(&engine.ruleSet, (Rule){4, "Advanced Controller Mounting Kit", "AdvancedController == 1 AND MountingKit == 0", "REQUIRE", "Mounting Hardware Kit", "If Advanced Controller is selected, Mounting Kit is required."});
    addRule(&engine.ruleSet, (Rule){5, "Low Voltage 15HP Incompatibility", "Motor15HP == 1 AND Voltage230 == 1", "INVALID", "230 V Power", "15 HP Motor + 230 V Power combination is INVALID."});

    // Initialize Graph Vertices
    addVertex(&engine.compatibilityGraph, 101, "Industrial Pump X", "Industrial Pumps");
    addVertex(&engine.compatibilityGraph, 103, "Industrial Motor A", "Electric Motors");
    addVertex(&engine.compatibilityGraph, 104, "Industrial Motor B", "Electric Motors");
    addVertex(&engine.compatibilityGraph, 201, "5 HP Motor", "Motor");
    addVertex(&engine.compatibilityGraph, 203, "15 HP Motor", "Motor");
    addVertex(&engine.compatibilityGraph, 205, "415 V Power", "Voltage");
    addVertex(&engine.compatibilityGraph, 204, "230 V Power", "Voltage");
    addVertex(&engine.compatibilityGraph, 209, "Advanced Controller", "Controller");
    addVertex(&engine.compatibilityGraph, 212, "Mounting Hardware Kit", "Mounting");

    // Initialize Graph Edges
    addEdge(&engine.compatibilityGraph, 101, 103, REL_COMPATIBLE, 1.0);
    addEdge(&engine.compatibilityGraph, 101, 104, REL_COMPATIBLE, 1.0);
    addEdge(&engine.compatibilityGraph, 103, 201, REL_COMPATIBLE, 1.0);
    addEdge(&engine.compatibilityGraph, 104, 203, REL_COMPATIBLE, 1.0);
    addEdge(&engine.compatibilityGraph, 203, 205, REL_REQUIRES, 2.0);
    addEdge(&engine.compatibilityGraph, 203, 204, REL_INCOMPATIBLE, 99.0);
    addEdge(&engine.compatibilityGraph, 209, 212, REL_REQUIRES, 1.0);

    QuoteEngine quoteEng;
    initQuoteEngine(&quoteEng);

    initAPI(&engine, &quoteEng);

    startHTTPServer(port);

    return 0;
}
