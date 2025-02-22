# Real-Time Graph App

This project is a real-time series graph application that displays data from six analog sensors connected to an Arduino via serial communication. The application updates the graph instantly with new sensor values and retains past data even after a system restart.

## Features

- Real-time updates of sensor data
- Graphical representation of sensor values
- Data persistence across system restarts
- User-friendly interface

## Project Structure

```
real-time-graph-app
├── src
│   ├── app.ts                  # Entry point of the application
│   ├── controllers
│   │   └── graphController.ts  # Handles graph-related requests
│   ├── routes
│   │   └── index.ts            # Sets up application routes
│   ├── services
│   │   └── serialService.ts     # Manages Arduino serial communication
│   ├── public
│   │   ├── index.html           # HTML structure for the web app
│   │   ├── styles.css           # CSS styles for the web app
│   │   └── scripts.js           # JavaScript for real-time updates
│   └── types
│       └── index.ts             # Type definitions for the application
├── package.json                 # npm configuration and dependencies
├── tsconfig.json                # TypeScript configuration
└── README.md                    # Project documentation
```

## Installation

1. Clone the repository:
   ```
   git clone <repository-url>
   cd real-time-graph-app
   ```

2. Install dependencies:
   ```
   npm install
   ```

3. Ensure you have the Arduino connected and configured to send data via serial.

## Usage

1. Start the application:
   ```
   npm start
   ```

2. Open your web browser and navigate to `http://localhost:3000` to view the real-time graph.

## Contributing

Contributions are welcome! Please open an issue or submit a pull request for any improvements or bug fixes.

## License

This project is licensed under the MIT License.