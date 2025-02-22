import { Router } from 'express';
import { GraphController } from '../controllers/graphController';
import { Server } from 'socket.io';

const router = Router();

export function setRoutes(app: Router, server: Server) {
    router.get('/', (req, res) => {
        res.sendFile('index.html', { root: 'public' });
    });

    const graphController = new GraphController(server);
    
    app.get('/api/sensors/current', graphController.getCurrentData.bind(graphController));
    app.get('/api/sensors/past', graphController.getPastData.bind(graphController));
}