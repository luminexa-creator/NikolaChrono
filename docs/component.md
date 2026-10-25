# Components List

Complete shopping list for building NikolaChrono.

> **Price notice:** All prices listed in this document are estimates in USD and may vary by supplier, region, availability, shipping costs, taxes, and import duties. Links are provided as search placeholders to help you find suitable components; verify specifications and compatibility before purchasing.

## Essential Components

The following 13 component categories contain the hardware required to build NikolaChrono. Check each specification before ordering, especially the ESP32 board connector type and the electrical compatibility of the display modules.

| # | Component | Spec | Qty | Est. Price (USD) | Notes |
|---:|---|---|---:|---:|---|
| 1 | **ESP32 DevKit V1** | ESP32-WROOM-32, `38-pin` or compatible `30-pin` | 1 | $5–$12 | Main microcontroller with Wi-Fi, BLE, GPIO, and FreeRTOS support. |
| 2 | **DS3231 RTC Module** | DS3231 with CR2032 battery holder and battery | 1 | $2–$5 | Provides accurate timekeeping and backup power. Must be powered from 3.3V for this project. |
| 3 | **TM1637 4-Digit Display** | `0.36-inch` or `0.56-inch`, four-digit, seven-segment | 1 | $1–$3 | Displays the time in HH:MM format. Select a module compatible with 3.3V logic and supply. |
| 4 | **16x2 LCD with I2C Module** | 16 characters × 2 lines, PCF8574 backpack | 1 | $3–$7 | Secondary display for alarm, weather, date, and stopwatch. Common I2C addresses are `0x27` and `0x3F`. |
| 5 | **Tactile Push Buttons** | `6x6 mm`, momentary, normally open | 5 | $0.50–$2 | Used for MODE, PLUS, MINUS, SNOOZE, and STOP. |
| 6 | **Active Buzzer** | `5V` active DC buzzer | 1 | $0.50–$2 | Provides the alarm sound. Use a suitable transistor driver if its current exceeds the ESP32 GPIO rating. |
| 7 | **5mm LED** | Standard indicator LED, any color | 1 | $0.05–$0.30 | Indicates seconds. Choose a common LED with a known forward voltage. |
| 8 | **220 Ohm Resistor** | `220Ω`, `1/4W`, through-hole | 1 | $0.02–$0.20 | Required in series with the LED to limit current. Buying a resistor assortment is recommended. |
| 9 | **Breadboard** | Solderless breadboard, approximately 830 tie points | 1 | $2–$5 | Allows circuit assembly without soldering. |
| 10 | **Jumper Wires** | Male-to-male and male-to-female Dupont wires | 1 set | $2–$5 | Used for breadboard connections and modules with header pins. |
| 11 | **USB Cable** | Micro-USB or USB-C, matching the ESP32 board | 1 | $1–$4 | Must support data transfer for programming, not charging only. |
| 12 | **5V Power Supply** | Regulated USB adapter, at least `5V 1A` | 1 | $3–$8 | Supplies the project through the ESP32 board's supported USB power input. |
| 13 | **Additional Resistors and Connectors** | Basic resistor assortment and compatible jumper connectors | 1 set | $1–$4 | Useful for prototyping, troubleshooting, and adapting module connections. |

**Important compatibility notes**

- The DS3231 RTC must connect to the ESP32's `3V3` pin, not `VIN` or 5V.
- The LCD's PCF8574 backpack may pull its I2C signals up to 5V. The ESP32 GPIO pins are not 5V tolerant. Verify the pull-up voltage and use a suitable bidirectional I2C level shifter if necessary.
- The TM1637 display must be compatible with 3.3V operation.
- The active buzzer must be suitable for the available supply and GPIO drive arrangement. A transistor driver may be required.
- The USB cable must match the connector fitted to your particular ESP32 DevKit.

## Optional Components

These components are not necessary for the initial prototype but can improve durability, assembly quality, and maintenance.

| # | Component | Spec | Qty | Est. Price (USD) | Notes |
|---:|---|---|---:|---:|---|
| 1 | **Perfboard or Custom PCB** | Through-hole perfboard or a PCB designed for the project | 1 | $1–$10 | Replaces the breadboard for a more permanent installation. |
| 2 | **Enclosure** | 3D-printed case or plastic electronics box | 1 | $3–$20 | Protects the circuit and gives the clock a finished appearance. |
| 3 | **Header Pins** | Standard 2.54mm male and female headers | 1 set | $0.50–$3 | Useful for soldering modules and making removable connections. |
| 4 | **Extra CR2032 Batteries** | 3V lithium coin cells | 2–5 | $1–$5 | Provides replacement batteries for the RTC backup circuit. |
| 5 | **Dupont Connectors** | 2.54mm housings, terminals, and connector assortment | 1 set | $1–$5 | Helps create tidy, reusable wiring harnesses. |
| 6 | **Heat Shrink Tubing** | Assorted diameters | 1 set | $1–$4 | Insulates solder joints and protects exposed conductors. |

## Estimated Total Cost

The following budget assumes one complete build, including the essential components and optional accessories.

| Category | Estimated Cost (USD) | Description |
|---|---:|---|
| Essential subtotal | $20–$57 | Core electronics, breadboard, wiring, and power supply. |
| Optional subtotal | $7.50–$47 | Enclosure, perfboard, connectors, spare batteries, and insulation. |
| Grand total | $27.50–$104 | Estimated complete project with optional accessories. |

These figures are planning estimates, not live quotations. Actual costs depend on the selected components and supplier.

### Factors Affecting Cost

- **AliExpress:** Often offers inexpensive modules and component bundles, but delivery may take longer.
- **Amazon:** Can offer faster shipping and convenient returns, depending on your location.
- **Local electronics shops:** May cost more per component but let you inspect parts and avoid international shipping.
- **Bulk purchases:** Resistor assortments, jumper-wire sets, and button packs can reduce the cost per component.
- **Shipping and import charges:** These can substantially increase the total for international orders, particularly when buying several inexpensive items separately.

For builders in Tanzania, compare the final delivered price with the cost of purchasing the components from local electronics suppliers.

## Where to Buy

The following links are search placeholders for finding compatible components. Check the specifications, seller ratings, delivery options, and total price before placing an order.

- **[AliExpress](https://www.aliexpress.com/)** — Often has low-cost ESP32 boards, RTC modules, displays, and electronic component bundles. Delivery times vary by seller and destination.

- **[Amazon](https://www.amazon.com/)** — Offers a broad selection of development boards, breadboards, cables, and starter kits. Availability and shipping costs depend on your region.

- **Local electronics shops** — Useful for buying jumper wires, resistors, LEDs, buttons, batteries, and breadboards immediately. Ask the seller to confirm the exact module specifications.

- **[Mouser Electronics](https://www.mouser.com/)** — Provides detailed datasheets, manufacturer part numbers, and components from established suppliers. It is particularly useful when precise electrical specifications matter.

- **[Digi-Key](https://www.digikey.com/)** — Offers a large electronic component catalog with technical documentation and manufacturer specifications. It is useful for sourcing verified parts and checking electrical ratings.

### Suggested Search Terms

| Component | Search Term |
|---|---|
| ESP32 | `ESP32 DevKit V1 ESP32-WROOM-32 30 pin 38 pin` |
| RTC | `DS3231 RTC module CR2032 battery` |
| Main display | `TM1637 4 digit 7 segment 0.36 inch` |
| LCD | `1602 LCD PCF8574 I2C backpack` |
| Buttons | `6x6mm tactile momentary normally open switch` |
| Buzzer | `5V active buzzer module` |
| LED and resistor | `5mm LED 220 ohm resistor 1/4W` |
| Breadboard | `830 tie point solderless breadboard` |
| Jumper wires | `Dupont jumper wires male male male female` |
| USB cable | `ESP32 DevKit USB data cable Micro USB USB C` |
| Power supply | `regulated 5V 1A USB power adapter` |

## Substitutions

Some components can be replaced with alternatives, but substitutions may require firmware changes, additional wiring, or different power arrangements.

| Original | Alternative | Impact |
|---|---|---|
| **DS3231 RTC** | DS1307 RTC module | Requires compatible RTC library and wiring. The DS1307 is generally less accurate and may require a different power arrangement. |
| **TM1637 display** | MAX7219-driven seven-segment display | Uses a different interface and requires different wiring and firmware. The MAX7219 module may use more GPIO signals through its serial interface. |
| **Active buzzer** | Passive buzzer | Requires a suitable alternating signal, usually generated using PWM or a tone function. Firmware changes are necessary. |
| **16x2 I2C LCD** | 20x4 I2C LCD | Offers additional display space but requires layout changes. Confirm the I2C address and signal-voltage compatibility. |
| **ESP32 DevKit V1** | ESP32-WROOM bare module | Requires a custom circuit with appropriate power regulation, boot circuitry, programming connections, and antenna clearance. Not recommended for beginners. |
| **Micro-USB cable** | USB-C cable | Works only if the selected ESP32 board has a USB-C connector. The cable must support data if used for programming. |
| **Breadboard** | Perfboard or custom PCB | Creates a more permanent assembly but requires soldering and careful verification of connections. |
| **Standard 5mm LED** | Different-color 5mm LED | Usually requires no firmware changes, but forward voltage varies. Verify that the 220Ω resistor provides a suitable current. |

### Substitution Guidelines

Before replacing a component:

1. Verify its operating voltage and current requirements.
2. Confirm that the replacement supports the required communication interface.
3. Check whether its GPIO assignments conflict with existing connections.
4. Update the firmware's pin definitions, libraries, or display configuration as needed.
5. Test the replacement independently before integrating it into the complete clock.

**Do not assume that two modules are electrically interchangeable simply because their connectors look similar.**

## Tools Required

### Required Tools and Equipment

- **Computer with Arduino IDE** — Required to configure, compile, and upload the ESP32 firmware.
- **USB port and compatible data cable** — Required to connect the ESP32 to the computer for programming and initial testing.
- **Small screwdriver** — Useful for adjusting the LCD contrast potentiometer and tightening compatible terminal connections.
- **Multimeter** — Strongly recommended for checking supply voltages, continuity, shorts, and electrical faults before powering the circuit.

### Optional Tools

- **Soldering iron and solder** — Required if you need to attach header pins, build a permanent circuit, or repair solder joints. Not necessary for a correctly assembled breadboard prototype with pre-soldered modules.
- **Wire strippers and cutters** — Useful for preparing bare wire and making clean connections. Pre-made Dupont wires can reduce the need for these tools.
- **Helping hands or PCB holder** — Useful for soldering small components.
- **Heat gun** — Useful for shrinking heat-shrink tubing; an appropriately controlled alternative may be used.
- **Digital calipers or ruler** — Helpful when designing a custom enclosure or PCB.

**Safety recommendation:** Disconnect USB power before modifying the wiring. Never use a multimeter's current-measurement mode directly across a power supply, and do not allow exposed conductors to short together.

## Package Diagram (text)

The following diagram illustrates the components needed for one NikolaChrono build. It is a logical inventory diagram, not a photograph or a physical assembly layout.

```text
+----------------------------------------------------------------+
|                        NIKOLACHRONO KIT                        |
|                                                                |
|  +----------------------+    +------------------------------+  |
|  | ESP32 DevKit V1      |    | DS3231 RTC Module            |  |
|  | Qty: 1               |    | Qty: 1                       |  |
|  | 30-pin / 38-pin      |    | Includes CR2032 battery      |  |
|  +----------------------+    +------------------------------+  |
|                                                                |
|  +----------------------+    +------------------------------+  |
|  | TM1637 Display       |    | 16x2 I2C LCD                 |  |
|  | Qty: 1               |    | Qty: 1                       |  |
|  | Four-digit display   |    | PCF8574 backpack             |  |
|  +----------------------+    +------------------------------+  |
|                                                                |
|  +----------------------+    +------------------------------+  |
|  | Tactile Buttons      |    | Active Buzzer                |  |
|  | Qty: 5               |    | Qty: 1                       |  |
|  | MODE / PLUS          |    | 5V DC                        |  |
|  | MINUS / SNOOZE / STOP|    +------------------------------+  |
|  +----------------------+                                       |
|                                                                |
|  +----------------------+    +------------------------------+  |
|  | 5mm LED              |    | 220 Ohm Resistor              |  |
|  | Qty: 1               |    | Qty: 1 minimum                |  |
|  +----------------------+    +------------------------------+  |
|                                                                |
|  +----------------------+    +------------------------------+  |
|  | Breadboard           |    | Jumper Wires                 |  |
|  | Qty: 1               |    | Qty: 1 set                   |  |
|  | 830 tie points       |    | M-M and M-F                  |  |
|  +----------------------+    +------------------------------+  |
|                                                                |
|  +----------------------+    +------------------------------+  |
|  | USB Data Cable       |    | 5V USB Power Adapter         |  |
|  | Qty: 1               |    | Qty: 1, 1A minimum            |  |
|  +----------------------+    +------------------------------+  |
|                                                                |
|  OPTIONAL ACCESSORIES                                          |
|  +----------------------+    +------------------------------+  |
|  | Perfboard / PCB      |    | Enclosure                    |  |
|  | Qty: 1               |    | Qty: 1                       |  |
|  +----------------------+    +------------------------------+  |
|                                                                |
|  +----------------------+    +------------------------------+  |
|  | Header Pins          |    | Extra CR2032 Batteries       |  |
|  | Qty: 1 set           |    | Qty: 2-5                     |  |
|  +----------------------+    +------------------------------+  |
|                                                                |
|  +----------------------+    +------------------------------+  |
|  | Dupont Connectors    |    | Heat Shrink Tubing           |  |
|  | Qty: 1 set           |    | Qty: 1 set                   |  |
|  +----------------------+    +------------------------------+  |
+----------------------------------------------------------------+
