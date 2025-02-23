import { SerialPort } from 'serialport';
import { ReadlineParser } from '@serialport/parser-readline';
import { EventEmitter } from 'events';

interface SensorData {
    sensor1: number;
    sensor2: number;
    sensor3: number;
    sensor4: number;
    sensor5: number;
    data_time: string;
}

export class SerialService extends EventEmitter {
    private port: SerialPort;
    private parser: ReadlineParser;
    private sensorData: SensorData[];

    constructor(portName: string) {
        super();
        this.port = new SerialPort({ path: portName, baudRate: 9600 });
        this.parser = this.port.pipe(new ReadlineParser({ delimiter: '\n' }));
        this.sensorData = [];

        this.initialize();
    }

    private initialize() {
        this.parser.on('data', (data: string) => {
            console.log(data)
            this.handleData(data);
        });

        this.port.on('error', (err) => {
            console.error('Error: ', err.message);
        });
    }

    private handleData(data: string) {
        const parsedData = this.parseSensorData(data);
        if (parsedData) {
            this.sensorData.push(parsedData);
            this.emit('data', parsedData);
            console.log(parsedData)
        }
    }

    private parseSensorData(data: string): SensorData | null {
        const values = data.split(',').map(Number);
        if (values.length === 5) {
            const dt = new Date();
            return {
                sensor1: values[0],
                sensor2: values[1],
                sensor3: values[2],
                sensor4: values[3],
                sensor5: values[4],
                data_time: dt.toISOString(),
            };
        }
        return null;
    }

    public getSensorData(): SensorData[] {
        return this.sensorData;
    }
}