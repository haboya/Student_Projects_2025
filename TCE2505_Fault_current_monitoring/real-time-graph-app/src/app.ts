import express from 'express';
import http from 'http';
import { Server } from 'socket.io';
import { setRoutes } from './routes/index';

const app = express();
const server = http.createServer(app);
const io = new Server(server);

app.use(express.json());
app.use(express.static(__dirname + '/public'));

setRoutes(app, io);

const PORT = process.env.PORT || 3000;

server.listen(PORT, () => {
    console.log(`Server is running on http://localhost:${PORT}`);
});