const express = require("express");
const mqtt = require("mqtt");

const app = express();

// Add CORS middleware
app.use((req, res, next) => {
  res.setHeader("Access-Control-Allow-Origin", "*");
  res.setHeader(
    "Access-Control-Allow-Methods",
    "GET, POST, OPTIONS, PUT, PATCH, DELETE"
  );
  res.setHeader(
    "Access-Control-Allow-Headers",
    "X-Requested-With,content-type"
  );
  res.setHeader("Access-Control-Allow-Credentials", true);
  next();
});

// Body parsing middleware
app.use(express.json());
app.use(express.urlencoded({ extended: true }));

// Define a variable to store the latest MQTT message
let latestMessage = "";

const mqttOptions = {
  host: "a2dfcf7d2fde4d719cbda072dd814b18.s1.eu.hivemq.cloud",
  port: 8883,
  protocol: "mqtts",
  username: "Handsome",
  password: "Handsome12345",
};

// Initialize the MQTT client
const mqttClient = mqtt.connect(mqttOptions);

// Setup MQTT client callbacks
mqttClient.on("connect", function () {
  console.log("Connected to MQTT broker");
});

mqttClient.on("error", function (error) {
  console.error("MQTT Error:", error);
});

// Express route to publish message
app.post("/publish", (req, res) => {
  const { topic, message } = req.body;
//   console.log(message);

  if (!topic || !message) {
    return res.status(400).json({ error: "Topic and message are required" });
  }
  // subscribe to topic 'my/test/topic'
  // client.subscribe('my/test/topic');
  // Publish message to the specified topic
  mqttClient.publish(topic, message, (err) => {
    if (err) {
      console.error("Error publishing message:", err);
      return res.status(500).json({ error: "Error publishing message" });
    }
    console.log("Published message to", topic);
    res.status(200).json({ message: "Message published successfully" });
  });
});

// Subscribe to the 'temp' topic
// mqttClient.on("message", function (topic, message) {
//     // Store the latest message
//     latestMessage = message.toString();
// });
// mqttClient.subscribe("temp");
// // for sub
// app.get("/sub", (req, res) => {
//     // res.send(latestMessage);
//     res.send({temp : latestMessage })
// });
// // mqttClient.subscribe("temp");

app.get("/", (req, res) => {
  res.send("Hello world!!!");
});

app.get("/home", (req , res) =>{
    res.sendFile('index.html');
})

// Start the Express server
const PORT = process.env.PORT || 3000;
app.listen(PORT, () => {
  console.log(`Server is running on port ${PORT}`);
});