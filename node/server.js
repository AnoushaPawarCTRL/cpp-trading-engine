const express = require("express");
const bodyParser = require("body-parser");
const { spawn } = require("child_process");

const app = express();
app.use(bodyParser.json());

let engineProcess = null;

function startEngine() {
    
}

const net = require("net");

app.post("/signal", (req, res) => {
    const { signal, qty, price } = req.body;

    const client = new net.Socket();

    client.connect(9000, "127.0.0.1", () => {
        const message = `${signal} ${qty} ${price}`;
        client.write(message);
        client.end();
    });

    console.log(`Sent to C++: ${signal} ${qty} @ ${price}`);

    res.send({ status: "sent to engine" });
});

app.listen(3000, () => {
    console.log("Node.js API listening on port 3000");
    startEngine();
});