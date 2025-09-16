// Send appropriate command to server
#include <WiFi.h>
#include <WebServer.h>
#include <Arduino.h>

// ---------------- WiFi + Web GUI ----------------
WebServer server(80);
const int LED_PIN = 2;
bool DeBug = false;
bool DeBug_Status = false;
uint8_t buffer[6] = { 0b10101010, 0b11110000, 0b00001111, 0b11001100, 0b00110011, 0b11111111 };

// Serial log buffer
String serialLog = "";

// ---------------- Function to Control Outputs ----------------
int s = 0;  // state (4 = ON, 3 = OFF)
char a = '0';
char b = '4';  // default output = 04

void turn_on_of() {
  Serial2.write('1');
  Serial2.write(',');
  Serial2.write('0');
  Serial2.write('4');
  Serial2.write('2');
  if (s == 4) {
    Serial2.write('4');  // ON
  }
  if (s == 3) {
    Serial2.write('5');  // OFF
  }
  Serial2.write(',');
  Serial2.write(a);
  Serial2.write(b);
  Serial2.write(0xF0);
}
void logBufferToGUI(uint8_t* buffer, size_t length) {
  String line = "";
  for (size_t i = 0; i < length; i++) {
    // Append each byte in hex format
    if (buffer[i] < 0x10) line += "0";  // padding
    line += String(buffer[i], HEX);
    line += " ";  // separator
  }
  line.trim();               // remove trailing space
  serialLog += line + "\n";  // append to log
}

// Parse received bytes
// read that bytes required for reading bluetooth data byte 1 to byte 6
void readbytes(int pos_1, String msg) {
  char a1, a2, a3, a4, a5, a6, a7, a8;
  Serial.println("5, recieved getting bytes from 6 to 14");
  a1 = msg.charAt(pos_1 + 6);
  a2 = msg.charAt(pos_1 + 7);
  a3 = msg.charAt(pos_1 + 8);
  a4 = msg.charAt(pos_1 + 9);
  a5 = msg.charAt(pos_1 + 10);
  a6 = msg.charAt(pos_1 + 11);
  a7 = msg.charAt(pos_1 + 12);
  a8 = msg.charAt(pos_1 + 13);
  //importnt for gui as it processes this buffer very easily
  //buffer that stores all 6 bytes for statuses
  // buffer[6]={a1,a2,a3,a4,a5,a6};

  buffer[0] = a1;
  buffer[1] = a2;
  buffer[2] = a3;
  buffer[3] = a4;
  buffer[4] = a5;
  buffer[5] = a6;





  logBufferToGUI(buffer, 6);

  //  SerialBT.write(buffer,6);



  //----------------------------------------------------------------------------------------------------------------
  //----------------------------------------------------------------------------------------------------------------
  // This below code maps all functions and send it to bluetooth in case mobile phone is attached uncomment below option
  //maps all statuses andeach time Rd is pressed it sends back these charecters


  //----------------------------------------------------------------------------------------------------------------
  //----------------------------------------------------------------------------------------------------------------


  //  SerialBT.println("Extracted Successfully");
  //for test
  Serial.println("|-----------------------------------------------|");
  Serial.println("| E22 | E21 | E19 | F44 | F06 | F10 | F15 | F18 |");
  for (int i = 7; i >= 0; i--) {
    Serial.print("|  ");
    Serial.print((a1 >> i) & 0x01);
    Serial.print("  ");
  }
  Serial.println("|");
  Serial.println("|-----------------------------------------------|");
  Serial.println("| F22 | F26 | C03 | C06 | C08 | C12 | F37 | F36 |");
  for (int i = 7; i >= 0; i--) {
    Serial.print("|  ");
    Serial.print((a2 >> i) & 0x01);
    Serial.print("  ");
  }
  Serial.println("|");
  Serial.println("|-----------------------------------------------|");
  Serial.println("| D02 | A19 | C10 | C09 | F12 | F08 | F43 | F35 |");
  for (int i = 7; i >= 0; i--) {
    Serial.print("|  ");
    Serial.print((a3 >> i) & 0x01);
    Serial.print("  ");
  }
  Serial.println("|");
  Serial.println("|-----------------------------------------------|");
  Serial.println("| E29 | E15 | E17 | E14 | E08 | E07 | E12 | B22 |");
  for (int i = 7; i >= 0; i--) {
    Serial.print("|  ");
    Serial.print((a4 >> i) & 0x01);
    Serial.print("  ");
  }
  Serial.println("|");
  Serial.println("|-----------------------------------------------|");
  Serial.println("| E31 | E30 | E25 | E24 | B19 | B16 | B14 | B13 |");
  for (int i = 7; i >= 0; i--) {
    Serial.print("|  ");
    Serial.print((a5 >> i) & 0x01);
    Serial.print("  ");
  }
  Serial.println("|");
  Serial.println("|-----------------------------------------------|");
  Serial.println("| F13 | F05 | F23 | F09 | B02 | A11 | E16 | E02 |");
  for (int i = 7; i >= 0; i--) {
    Serial.print("|  ");
    Serial.print((a6 >> i) & 0x01);
    Serial.print("  ");
  }
  Serial.println("|");
  Serial.println("|-----------------------------------------------|");
}

// ==================== SEND READ COMMAND ====================
void Inputs_Cal() {
  Serial2.write('1');
  Serial2.write(',');
  Serial2.write('0');
  Serial2.write('4');
  Serial2.write('2');
  Serial2.write('1');
  Serial2.write(',');
  Serial2.write('A');
  Serial2.write(0xF0);
  Serial.println("Read command sent.");
}
// ==================== SERIAL RECEIVE ====================
void readTempData() {
  while (Serial2.available()) {
    String Seri = Serial2.readStringUntil('\r');
    Serial.println(Seri);


    //  SerialBT.println(Seri);


    // if (Seri.indexOf("5,úúúú") >= 0){
    //     int pos_1 = Seri.indexOf("5,");

    int pos_1 = Seri.indexOf("5,");
    //gets postion and string as readbytes has to decode these bytes and send to bluetooth
    if (pos_1 != -1) {
      readbytes(pos_1, Seri);
    }
    // reads position as we need bytes after 5,

    //gets postion and string as readbytes has to decode these bytes and send to bluetooth

    //  readbytes(pos_1, Seri);
  }
}


// ---------------- HTML page ----------------
const char HTML_PAGE[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>

<head>
    <title>SSAL GUI</title>
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <style>
        @import url('https://fonts.googleapis.com/css2?family=Red+Hat+Display:ital,wght@0,300..900;1,300..900&display=swap');

        :root {
            --bg-color-main: #111827;
            --bg-color-card: #1f2937;
            --card-color: #374151;
            --text-color: #f9fafb;
            --btn-output-off: #6c7aa0;
            --btn-output-on: #1d4ed8;
            --btn-output-active: #2d7327;
            --btn-read-on: #a70d0d;
            --btn-read-off: #2d7327;
            --btn-debug-on: #2d7327;
            --btn-debug-off: #6c7aa0;
            --led-off: #5a5a5a;
            --led-on: #3ee54c;
        }

        body {
            font-family: "Red Hat Display", sans-serif;
            font-optical-sizing: auto;
            margin: 0;
            background: linear-gradient(135deg, var(--bg-color-card), var(--bg-color-main));
            color: var(--text-color);
            min-height: 100vh;
        }

        h1 {
            text-align: center;
            color: var(--text-color);
            margin: 2px 0;
            font-size: clamp(1.5rem, 4vw, 2.5rem);
        }

        h2,
        h3,
        h4 {
            margin: 10px;
            color: var(--text-color);
            font-weight: 500;
        }

        .container {
            display: flex;
            flex-wrap: wrap;
            gap: 15px;
            padding: 15px;
            justify-content: center;
        }

        .card {
            background: var(--card-color);
            padding: 20px;
            border-radius: 12px;
            box-shadow: 0 4px 12px rgba(0, 0, 0, 0.4);
            flex: 1 1 300px;
            /* Allows cards to grow and shrink */
            display: flex;
            flex-direction: column;
        }

        .btn-grid {
            display: grid;
            grid-template-columns: repeat(auto-fit, minmax(80px, 1fr));
            gap: 15px;
            margin-top: 15px;
        }

        .output-btn,
        .r-btn {
            width: 100%;
            padding: 12px 8px;
            border: none;
            background: var(--btn-output-off);
            color: white;
            border-radius: 8px;
            cursor: pointer;
            transition: transform 0.2s ease, background-color 0.2s ease;
            box-shadow: rgba(255, 255, 255, 0.1) 0px 2px 8px 0px;
        }

        .output-btn:hover,
        .r-btn:hover {
            transform: translateY(-2px);
        }

        .output-btn.on {
            background-color: var(--btn-output-active);
        }

        .r-btn:hover {
            background: #4b5563;
        }

        .right-btns {
            display: flex;
            /* flex-wrap: wrap; */
            flex-direction: row;
            gap: 10px;
            margin-top: 15px;
            justify-content: space-around;
        }

        #ledBtn.on {
            background-color: var(--btn-output-active);
        }

        #readBtn.on {
            background-color: var(--btn-read-on);
        }

        #readBtn.off {
            background-color: var(--btn-read-off);
        }

        #debugBtn.on {
            background-color: var(--btn-debug-on);
        }

        #debugBtn.off {
            background-color: var(--btn-debug-off);
        }

        .log-box {
            background: #000;
            border: 1px solid #374151;
            height: 350px;
            padding: 15px;
            border-radius: 8px;
            font-family: monospace;
            font-size: 14px;
            box-shadow: inset 0 2px 6px rgba(0, 0, 0, 0.5);
            color: #0f0;
            overflow-y: auto;
            flex-grow: 1;
        }

        #log {
            white-space: pre-wrap;
            /* so \n creates line breaks */
            font-family: monospace;
            color: #0f0;
        }


        .led-grid {
            display: flex;
            flex-wrap: wrap;
            gap: 10px;
            margin-top: 20px;
            justify-content: center;
        }

        .led-box {
            display: flex;
            flex-direction: column;
            align-items: center;
            text-align: center;
            flex: 0 0 calc(12% - 12px);
            min-width: 60px;
        }

        .led {
            width: 40px;
            height: 40px;
            border-radius: 50%;
            background-color: var(--led-off);
            display: flex;
            align-items: center;
            justify-content: center;
            color: var(--text-color);
            font-size: 12px;
            box-shadow: 0 0 5px rgba(0, 0, 0, 0.5);
        }

        .led.on {
            background-color: var(--led-on);
            box-shadow: 0 0 15px var(--led-on), 0 0 5px var(--led-on) inset;
        }

        .led-label {
            color: var(--text-color);
            font-weight: bold;
        }
    </style>
</head>

<body>
    <div class="container">
        <div class="card">
            <h3>Outputs</h3>
            <div class="btn-grid">
                <button class="output-btn" id="A4" onclick="toggleOutput('04',id)">A4</button>
                <button class="output-btn" id="A8" onclick="toggleOutput('07',id)">A8</button>
                <button class="output-btn" id="E5" onclick="toggleOutput('02',id)">E5</button>
                <button class="output-btn" id="E3" onclick="toggleOutput('01',id)">E3</button>
                <button class="output-btn" id="B5" onclick="toggleOutput('00',id)">B5</button>
                <button class="output-btn" id="SH1" onclick="toggleOutput('35',id)">SH1</button>
                <button class="output-btn" id="A9" onclick="toggleOutput('06',id)">A9</button>
                <button class="output-btn" id="A18" onclick="toggleOutput('09',id)">A18</button>
                <button class="output-btn" id="MF2" onclick="toggleOutput('26',id)">MF2</button>
                <button class="output-btn" id="MH2" onclick="toggleOutput('82',id)">MH2</button>
                <button class="output-btn" id="SH2" onclick="toggleOutput('34',id)">SH2</button>
                <button class="output-btn" id="SC2" onclick="toggleOutput('32',id)">SC2</button>
                <button class="output-btn" id="SC1" onclick="toggleOutput('33',id)">SC1</button>
                <button class="output-btn" id="MC3" onclick="toggleOutput('81',id)">MC3</button>
                <button class="output-btn" id="MR3" onclick="toggleOutput('16',id)">MR3</button>
                <button class="output-btn" id="MW1" onclick="toggleOutput('25',id)">MW1</button>
                <button class="output-btn" id="MG2" onclick="toggleOutput('20',id)">MG2</button>
                <button class="output-btn" id="MG1" onclick="toggleOutput('21',id)">MG1</button>
                <button class="output-btn" id="MC2" onclick="toggleOutput('44',id)">MC2</button>
                <button class="output-btn" id="MC3" onclick="toggleOutput('45',id)">MC3</button>
                <button class="output-btn" id="BOOT" onclick="toggleOutput('00',id)">BOOT</button>
                <button class="output-btn" id="PWR" onclick="toggleOutput('00',id)">PWR</button>
                <button class="output-btn" id="MEM" onclick="toggleOutput('00',id)">MEM</button>
                <button class="output-btn" id="AMO" onclick="toggleOutput('00',id)">AMO</button>
                <button class="output-btn" id="CANP" onclick="toggleOutput('00,',id)">CANP</button>
                <button class="output-btn" id="SF1" onclick="toggleOutput('00',id)">SF1</button>
            </div>
        </div>

        <div class="card">
            <div class="btn-grid" style="margin-top: 0;">
                <h3>Console Log</h3>
                <button class="output-btn" style="width: 70%; margin: 0 0; height: 85%;" onclick="SaveLogs()">Save
                    Logs</button>
            </div>
            <div id="consoleLog" class="log-box">
                <div id="log"></div>
            </div>
        </div>

        <div class="card">
            <h3>Inputs</h3>
            <div class="right-btns">
                <button id="debugBtn" class="r-btn off" onclick="toggleDebug()">Enable Debug</button>
                <button id="readBtn" class="r-btn off" onclick="startRead()">READ</button>
                <button id="ledBtn" class="r-btn off" onclick="toggleLED()">LED</button>
            </div>
            <div class="led-grid" id="ledGrid"></div>
        </div>
    </div>


    <script>
        let ledState = false;
        let outputStates = {}; // Track all outputs dynamically
        let setReadStatus = false;
        let readInterval = null;

        let isSavingLogs = false;
        let saveLogInterval = null;
        let logBuffer = [];
        let lastLogLength = 0;

        async function toggleLED() {
            try {
                const btn = document.getElementById("ledBtn");
                const res = await fetch('/statusLED');
                const text = await res.text();
                ledState = text.includes("ON");
                await fetch(ledState ? '/off' : '/on');
                btn.classList.toggle('on', !ledState);
                btn.textContent = !ledState ? "LED ON" : "LED OFF";
            } catch (e) {
                alert("LED toggle failed: " + e);
            }
        }

        async function toggleOutput(out, id) {
            try {
                const outputBtn = document.getElementById(id);
                const currentState = outputStates[out] || false;
                const newState = !currentState;
                outputStates[out] = newState;
                outputBtn.classList.toggle('on', newState);

                const cmd = currentState ? `/outputOff?out=${out}` : `/outputOn?out=${out}`;
                await fetch(cmd);
            } catch (e) {
                alert(`Toggle failed for output ${out}`);
                const outputBtn = document.getElementById(id);
                outputBtn.classList.remove('on');
            }
        }

        async function updateDebugButton() {
            try {
                const res = await fetch('/debugStatus');
                const data = await res.json();
                const btn = document.getElementById("debugBtn");
                btn.classList.toggle('on', data.debug);
                btn.textContent = data.debug ? "Debug ON" : "Debug OFF";
            } catch (e) {
                console.error("Failed to update debug button status:", e);
            }
        }

        async function toggleDebug() {
            try {
                await fetch('/debug');
                updateDebugButton();
            } catch (e) {
                console.error("Failed to toggle debug:", e);
            }
        }

        async function updateLog() {
            try {
                const res = await fetch('/log');
                const text = await res.text();

                const logEl = document.getElementById('log');
                const scrollContainer = document.getElementById('consoleLog');

                logEl.innerText += "\n" + text;

                // Scroll the actual scrollable container
                scrollContainer.scrollTop = scrollContainer.scrollHeight;

            } catch (e) {
                const logEl = document.getElementById('log');
                const scrollContainer = document.getElementById('consoleLog');

                logEl.textContent += "\nLog disconnected";
                scrollContainer.scrollTop = scrollContainer.scrollHeight;
            }
        }

        // Saving Logs on clicking the button and Downloading the file

        // function SaveLogs() {
        //     const logElement = document.getElementById('log');
        //     const logs = logElement.innerText.trim();

        //     if (!logs) {
        //         alert("No logs to save.");
        //         return;
        //     }

        //     const blob = new Blob([logs], { type: 'text/plain' });
        //     const url = URL.createObjectURL(blob);

        //     const link = document.createElement('a');
        //     link.href = url;
        //     link.download = `console_logs_${new Date().toISOString().replace(/[:.]/g, '-')}.txt`;
        //     document.body.appendChild(link);
        //     link.click();
        //     document.body.removeChild(link);
        //     URL.revokeObjectURL(url); // Cleanup
        // }


        // Saving the File on the duration (Buffer)

        function SaveLogs() {
            const button = document.querySelector('button[onclick="SaveLogs()"]');

            if (!isSavingLogs) {
                // Start saving logs
                logBuffer = [];
                isSavingLogs = true;
                lastLogLength = 0;
                button.textContent = "Stop Saving Logs";

                saveLogInterval = setInterval(() => {
                    const logElement = document.getElementById('log');
                    const fullLogs = logElement.innerText.trim().split('\n');

                    // Get only new lines since lastLogLength
                    const newLines = fullLogs.slice(lastLogLength);
                    lastLogLength = fullLogs.length;

                    if (newLines.length > 0) {
                        logBuffer.push(...newLines.map(line => `[${new Date().toLocaleTimeString()}] ${line}`));
                    }
                }, 1000);

            } else {
                // Stop saving and download the file
                isSavingLogs = false;
                clearInterval(saveLogInterval);
                button.textContent = "Save Logs";

                if (logBuffer.length === 0) {
                    alert("No logs were captured.");
                    return;
                }

                const blob = new Blob([logBuffer.join('\n')], { type: 'text/plain' });
                const url = URL.createObjectURL(blob);

                const link = document.createElement('a');
                link.href = url;
                link.download = `console_logs_${new Date().toISOString().replace(/[:.]/g, '-')}.txt`;
                document.body.appendChild(link);
                link.click();
                document.body.removeChild(link);
                URL.revokeObjectURL(url);
            }
        }

        async function startRead() {
            try {
                const btn = document.getElementById("readBtn");
                setReadStatus = !setReadStatus;
                if (setReadStatus) {
                    btn.classList.add('on');
                    btn.textContent = "STOP";
                    readInterval = setInterval(sendReadCommand, 100);
                } else {
                    btn.classList.remove('on');
                    btn.textContent = "READ";
                    clearInterval(readInterval);
                    readInterval = null;
                }
            } catch (e) {
                alert("Exception Caught: " + e);
            }
        }

        async function sendReadCommand() {
            try {
                let res = await fetch('/read');
                let text = await res.text();
                console.log("READ response:", text);
            } catch (e) {
                alert("READ command failed");
            }
        }


        // LED Grid Logic
        const ledNames = ["F18", "F15", "F10", "F06", "F44", "E19", "E21", "E22", "F36", "F37", "C12", "C06", "C06", "C03", "F26", "F22", "F35", "F43", "F08", "F12", "C09", "C10", "A19", "D02", "B22", "E12", "E07", "E08", "E14", "E17", "E15", "E29", "B13", "B14", "B16", "B19", "E24", "E25", "E30", "E31", "E02", "E16", "A11", "B02", "F09", "F23", "F05", "F13"];
        const grid = document.getElementById('ledGrid');

        function createLedElements() {
            grid.innerHTML = '';
            for (let i = 0; i < 48; i++) {
                const wrapper = document.createElement('div');
                wrapper.className = 'led-box';

                const div = document.createElement('div');
                div.className = 'led';
                div.id = 'led' + i;

                // Put text inside the LED circle
                div.innerText = ledNames[i] || `LED ${i + 1}`;

                wrapper.appendChild(div);
                grid.appendChild(wrapper);
            }
        }


        async function refreshLedStatus() {
            try {
                const res = await fetch('/status');
                const bits = await res.text();
                for (let i = 0; i < bits.length && i < 48; i++) {
                    const led = document.getElementById('led' + i);
                    if (led) {
                        led.classList.toggle('on', bits[i] === '1');
                    }
                }
            } catch (e) {
                console.log("Status read failed:", e);
            }
        }

        // Initial setup and regular updates
        window.onload = () => {
            createLedElements();
            updateDebugButton();
            refreshLedStatus();
            updateLog();
        };

        setInterval(refreshLedStatus, 100);
        setInterval(updateLog, 1000);
    </script>
</body>

</html>
)rawliteral";

// ---------------- Serial Communication ----------------
String inputBuffer = "";
bool messageComplete = false;
#define RXD2 16
#define TXD2 17
#define UART_BAUD 115200

// ---------------- Web Handlers ----------------
void handleRoot() {
  server.send(200, "text/html", HTML_PAGE);
}

void blinkConnectionLED(int times = 1, int delayMs = 100) {
  for (int i = 0; i < times; i++) {
    digitalWrite(LED_PIN, HIGH);
    delay(delayMs);
    digitalWrite(LED_PIN, LOW);
    delay(delayMs);
  }
}

void handleLedOn() {
  digitalWrite(LED_PIN, HIGH);
  blinkConnectionLED();
  server.send(200, "text/plain", "OK");
}

void handleLedOff() {
  digitalWrite(LED_PIN, LOW);
  blinkConnectionLED();
  server.send(200, "text/plain", "OK");
}

void handleLEDStatus() {
  if (digitalRead(LED_PIN) == HIGH) {
    server.send(200, "text/plain", "LED is ON");
  } else {
    server.send(200, "text/plain", "LED is OFF");
  }

  // if (DeBug) {
  //   server.send(200, "text/plain", "DeBug Enabled");
  // } else {
  //   server.send(200, "text/plain", "DeBug Disabled");
  // }
}

void handleLog() {
  server.send(200, "text/plain", serialLog);
}

// void handleDebug() {
//   if(DeBug_Status)
//   {
//     DeBug = false;
//     DeBug_Status = false;
//     server.send(400, "text/plain", "Don't Reset PD-Box");
//   }
//   else
//   {
//     DeBug = true;
//     server.send(200, "text/plain", "Reset PD-Box");
//   }
// }

void handleDebug() {
  DeBug = !DeBug;
  DeBug_Status = DeBug;
  server.send(200, "text/plain", DeBug ? "Debug Enabled" : "Debug Disabled");
}

void handleDebugStatus() {
  if (DeBug_Status) {
    server.send(200, "application/json", "{\"debug\":true}");
  } else {
    server.send(200, "application/json", "{\"debug\":false}");
  }
}

void handleOutputOn() {
  if (server.hasArg("out")) {
    String out = server.arg("out");
    a = out[0];
    b = out[1];
    s = 4;  // ON
    turn_on_of();
    server.send(200, "text/plain", "Output " + out + " turned ON");
  } else {
    server.send(400, "text/plain", "Missing output number");
  }
}

void handleOutputOff() {
  if (server.hasArg("out")) {
    String out = server.arg("out");
    a = out[0];
    b = out[1];
    s = 3;  // OFF
    turn_on_of();
    server.send(200, "text/plain", "Output " + out + " turned OFF");
  } else {
    server.send(400, "text/plain", "Missing output number");
  }
}

void handleRead() {
  Inputs_Cal();
  blinkConnectionLED();
  server.send(200, "text/plain", "Read Command Sent, check Serial Monitor");

  readTempData();  // always check Serial2
}
////////////////////////////////////////////////////////
//uint8_t buffer[6];  // yeh MCU se aata hai

String getStatusBits() {
  String s = "";
  for (int i = 0; i < 6; i++) {
    for (int b = 0; b < 8; b++) {
      if (buffer[i] & (1 << b)) s += "1";
      else s += "0";
    }
  }
  return s;  // e.g. "101100...." length 48
}

void handleStatus() {
  server.send(200, "text/plain", getStatusBits());
}


// ---------------- Setup ----------------
void setup() {
  pinMode(LED_PIN, OUTPUT);
  Serial.begin(115200);
  Serial2.begin(UART_BAUD, SERIAL_8N1, RXD2, TXD2);

  WiFi.softAP("ESP32_AP", "12345678");
  Serial.println("ESP32 running as Access Point");
  Serial.print("AP IP address: ");
  Serial.println(WiFi.softAPIP());

  server.on("/", handleRoot);
  server.on("/on", handleLedOn);
  server.on("/off", handleLedOff);
  server.on("/status", handleStatus);
  server.on("/statusLED", handleLEDStatus);

  server.on("/log", handleLog);
  server.on("/debug", handleDebug);
  server.on("/debugStatus", handleDebugStatus);
  server.on("/outputOn", handleOutputOn);
  server.on("/outputOff", handleOutputOff);

  server.on("/read", handleRead);

  server.begin();
  Serial.println("Web server started");
}

// ---------------- Loop ----------------
void loop() {
  server.handleClient();

  while (Serial2.available()) {
    char c = (char)Serial2.read();
    inputBuffer += c;

    if (c == '\r') {
      messageComplete = true;
    }
  }

  if (messageComplete) {
    Serial.print("Received: ");
    Serial.println(inputBuffer);

    serialLog += inputBuffer + "\n";
    if (serialLog.length() > 5000) {
      serialLog.remove(0, 1000);
    }

    // Debug triggers
    if ((inputBuffer.indexOf("5,0000,Reset") >= 0) && DeBug) {
      Serial.println("Match Found! Sending special chars...");
      for (int i = 0; i < 4; i++) {
        Serial2.write(0xFE);
      }
    }

    if (inputBuffer.indexOf("5,0000,DbgOn") >= 0) {
      Serial.println("Debug Mode Enabled!");
      DeBug_Status = true;
    }

    inputBuffer = "";
    messageComplete = false;
  }
}