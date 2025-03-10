export interface SensorData {
    id: number;
    value: number;
    timestamp: Date;
}

export interface SensorDataResponse {
    current: SensorData[];
    past: SensorData[];
}