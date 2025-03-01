import { Request, Response } from 'express';
import { SerialService } from '../services/serialService';
import { Server } from 'socket.io';

export class GraphController {
    private serialService: SerialService;
    private io: Server;

    constructor(io: Server) {
        this.io = io;
        this.serialService = new SerialService("COM4");
        
        // Add listener for serial data
        this.serialService.on('data', (data) => {
            // Emit the data to all connected clients through socket.io
            this.io.emit('data', data);
        });
    }

    public getCurrentData(req: Request, res: Response): void {
        const currentData = this.serialService.getSensorData();
        res.json(currentData);
    }

    public getPastData(req: Request, res: Response): void {
        const pastData = this.serialService.getSensorData();
        res.json(pastData);
    }
}