import { SerialPort } from 'serialport';
import { ReadlineParser } from '@serialport/parser-readline';
const port = new SerialPort({ path: 'COM1', baudRate: 9600 });
const parser = port.pipe(new ReadlineParser({ delimiter: '\n' }));

let data_count = 0;

function generateSensorData() {
    const sensorRanges = [
        { min: 72.0, max: 75.0 },    // Sensor 1: 0-5A range
        { min: 44.0, max: 46.0 },   // Sensor 2: 2-10A range
        { min: 35.0, max: 36.0 },   // Sensor 3: 5-15A range
        { min: 22.0, max: 24.0 },   // Sensor 4: 8-18A range
        { min: 15.0, max: 17.0 }   // Sensor 5: 10-20A range
    ];

    return sensorRanges.map(range => {
        const value = Math.random() * (range.max - range.min) + range.min;
        return value.toFixed(2);
    });
}

port.on('open', () => {
    console.log('Serial port opened');
    setInterval(() => {
        const sensorData = generateSensorData();
        data_count++
        if(data_count >= 30)
        {
            switch (data_count) 
            {
                case 30:
                    sensorData[4] = 1219.31;
                    break;
                case 31:
                    sensorData[4] = 1313.19;
                    break;
                case 32:
                    sensorData[4] = 218.95;
                    break;
            
                default:
                    sensorData[0] = 0.0;
                    sensorData[1] = 0.0;
                    sensorData[2] = 0.0;
                    sensorData[3] = 0.0;
                    sensorData[4] = 0.0;
                    break;
            }

        }

        const dataString = sensorData.join(',');
        port.write(dataString + '\n', (err) => {
            if (err) {
                return console.log('Error on write: ', err.message);
            }
            console.log(`Sent: ${dataString} at ${data_count}`);
        });
    }, 411);
});

process.on('SIGINT', () => {
    console.log('Program interrupted by user');
    port.close(() => {
        console.log('Serial port closed');
        process.exit();
    });
});