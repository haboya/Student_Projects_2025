import { SerialPort } from 'serialport';
import { ReadlineParser } from '@serialport/parser-readline';
const port = new SerialPort({ path: 'COM1', baudRate: 9600 });
const parser = port.pipe(new ReadlineParser({ delimiter: '\n' }));

function generateSensorData() {
    return Array.from({ length: 5 }, () => (Math.random() * 20).toFixed(2));
}

port.on('open', () => {
    console.log('Serial port opened');
    setInterval(() => {
        const sensorData = generateSensorData();
        const dataString = sensorData.join(',');
        port.write(dataString + '\n', (err) => {
            if (err) {
                return console.log('Error on write: ', err.message);
            }
            console.log(`Sent: ${dataString}`);
        });
    }, 1000);
});

process.on('SIGINT', () => {
    console.log('Program interrupted by user');
    port.close(() => {
        console.log('Serial port closed');
        process.exit();
    });
});