const socket = io(); // Assuming socket.io is being used for real-time communication

const sensorData = {
    labels: [],
    datasets: [
        {
            data: [],
        },
        {
            data: [],
        },
        {
            data: [],
        },
        {
            data: [],
        },
        {
            data: [],
        },
    ]
};

const RELAY_OFF_THESHOLD = 0.1;
let new_data_update = false;
let pause_chart_update = false;
let droped_count = 0

function toggleChartUpdate()
{
    pause_chart_update = !pause_chart_update;
    if(pause_chart_update){
        document.getElementById('pause-btn').innerText = 'Resume';
        document.getElementById('pause-btn').style.color = 'white';
        document.getElementById('pause-btn').style.backgroundColor = '#5d1e02';
    }
    else{
        document.getElementById('pause-btn').innerText = 'Pause';
        document.getElementById('pause-btn').style.color = 'black';
        document.getElementById('pause-btn').style.backgroundColor = 'white';
    }
}

function appendSensorData( data_array)
{
    if(droped_count > 21 && data_array.sensor1 < 0.2){
        return;
    }

    if(data_array.sensor1 < 0.1)
    {
        droped_count++;
    }
    else{
        droped_count = 0;
    }
    sensorData.labels.push(data_array.data_time);
    sensorData.datasets[0].data.push(data_array.sensor1);
    sensorData.datasets[1].data.push(data_array.sensor2);
    sensorData.datasets[2].data.push(data_array.sensor3);
    sensorData.datasets[3].data.push(data_array.sensor4);
    sensorData.datasets[4].data.push(data_array.sensor5);
    // console.log(data_array)

    // Limit the number of data points to display
    if (sensorData.labels.length > 45) {
        sensorData.labels.shift();
        sensorData.datasets[0].data.shift();
        sensorData.datasets[1].data.shift();
        sensorData.datasets[2].data.shift();
        sensorData.datasets[3].data.shift();
        sensorData.datasets[4].data.shift();
    }
}

function updateRelayStatus()
{
    const index = sensorData.labels.length - 1;
    document.getElementById('relay1-value').innerText = sensorData.datasets[0].data[index] + ' A';
    document.getElementById('relay2-value').innerText = sensorData.datasets[1].data[index] + ' A';
    document.getElementById('relay3-value').innerText = sensorData.datasets[2].data[index] + ' A';
    document.getElementById('relay4-value').innerText = sensorData.datasets[3].data[index] + ' A';
    document.getElementById('relay5-value').innerText = sensorData.datasets[4].data[index] + ' A';
    
    if(sensorData.datasets[0].data[index] < RELAY_OFF_THESHOLD) {
        document.getElementById('relay1-state').checked = false;
    } else {
        document.getElementById('relay1-state').checked = true;
    }
    if(sensorData.datasets[1].data[index] < RELAY_OFF_THESHOLD) {
        document.getElementById('relay2-state').checked = false;
    } else {
        document.getElementById('relay2-state').checked = true;
    }
    if(sensorData.datasets[2].data[index] < RELAY_OFF_THESHOLD) {
        document.getElementById('relay3-state').checked = false;
    } else {
        document.getElementById('relay3-state').checked = true;
    }
    if(sensorData.datasets[3].data[index] < RELAY_OFF_THESHOLD) {
        document.getElementById('relay4-state').checked = false;
    } else {
        document.getElementById('relay4-state').checked = true;
    }
    if(sensorData.datasets[4].data[index] < RELAY_OFF_THESHOLD) {
        document.getElementById('relay5-state').checked = false;
    } else {
        document.getElementById('relay5-state').checked = true;
    }
}

function openChartView()
{
    document.getElementById('chart-container').style.display = "block";
    document.getElementById('raw-values').style.display = "none";
}

function closeChartView()
{
    document.getElementById('chart-container').style.display = "none";
    document.getElementById('raw-values').style.display = "block";
}

function downloadData() {
    // Convert sensor data to CSV format
    let csv = 'Time,Sensor1,Sensor2,Sensor3,Sensor4,Sensor5\n';
    
    for (let i = 0; i < sensorData.labels.length; i++) {
        csv += `${sensorData.labels[i]},`;
        csv += `${sensorData.datasets[0].data[i]},`;
        csv += `${sensorData.datasets[1].data[i]},`;
        csv += `${sensorData.datasets[2].data[i]},`;
        csv += `${sensorData.datasets[3].data[i]},`;
        csv += `${sensorData.datasets[4].data[i]}\n`;
    }

    // Create a Blob containing the CSV data
    const blob = new Blob([csv], { type: 'text/csv' });
    
    // Create a temporary download link
    const url = window.URL.createObjectURL(blob);
    const a = document.createElement('a');
    a.setAttribute('hidden', '');
    a.setAttribute('href', url);
    a.setAttribute('download', 'sensor_data.csv');
    document.body.appendChild(a);
    
    // Trigger the download
    a.click();
    
    // Clean up
    window.URL.revokeObjectURL(url);
    document.body.removeChild(a);
}

const ctx = document.getElementById('sensorGraph').getContext('2d');
const sensorChart = new Chart(ctx, {
    type: 'line',
    data: {
        labels: sensorData.labels,
        datasets: [
            {
                label: 'Bus14DR',
                data: sensorData.datasets[0].data,
                borderColor: 'rgba(75, 192, 192, 1)',
                // backgroundColor: 'rgba(75, 192, 192, 0.2)',
                fill: true,
            },
            {
                label: 'Bus15',
                data: sensorData.datasets[1].data,
                borderColor: 'rgba(192, 75, 75, 1)',
                // backgroundColor: 'rgba(192, 75, 75, 0.2)',
                fill: true,
            },
            {
                label: 'Bus16',
                data: sensorData.datasets[2].data,
                borderColor: 'rgba(75, 75, 192, 1)',
                // backgroundColor: 'rgba(75, 75, 192, 0.2)',
                fill: true,
            },
            {
                label: 'Bus22',
                data: sensorData.datasets[3].data,
                borderColor: 'rgba(192, 192, 75, 1)',
                // backgroundColor: 'rgba(192, 192, 75, 0.2)',
                fill: true,
            },
            {
                label: 'Bus24',
                data: sensorData.datasets[4].data,
                borderColor: 'rgb(172, 168, 242)',
                // backgroundColor: 'rgba(192, 192, 75, 0.2)',
                fill: true,
            }
        ]
    },
    options: {
        scales: {
            x: {
                type: 'time',
                time: {
                    unit: 'millisecond'
                },
                title: {
                    display: true,
                    text: 'Timestamp'
                }
            },
            y: {
                title: {
                    display: true,
                    text: 'Current (Amps)'
                }
            }
        },
        plugins: {
            zoom: {
                zoom: {
                    wheel: {
                        enabled: true,
                    },
                    pinch: {
                        enabled: true
                    },
                    mode: 'xy',
                }
            }
        }
    }
});

// Request old data once the chart is fully loaded
fetch('/api/sensors/past')
    .then(response => response.json())
    .then(oldData => {
        oldData.forEach(dataPoint => {
            // sensorData.labels.push(dataPoint.data_time);
            // sensorData.datasets[0].data.push(dataPoint.sensor1);
            appendSensorData(dataPoint);
        });

        sensorChart.update();
    })
    .catch(error => console.error('Error fetching old data:', error));

// Listen for updates from the server
socket.on('data', (newData) => {
    appendSensorData(newData);
    updateRelayStatus();
    // sensorChart.update();
    new_data_update = true;
});


const updateChartData = setInterval(()=>{

    if(new_data_update == true && !pause_chart_update){
        sensorChart.update();
        new_data_update = false;
    }
}, 5000);


/* ----------------------------------- EOF ---------------------------------- */