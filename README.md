# OpenMonitor

OpenMonitor is an open-source monitoring device based on the PlatformIO framework for Arduino. While initially designed for environmental analysis, it can work with any sensor compatible with the ESP32, allowing for flexible and customizable data collection.

## Features

- **Modular Sensor System**: Easily add or remove sensors by editing a configuration file.
- **InfluxDB Integration**: Log sensor data to an InfluxDB instance for powerful data analysis and visualization.
- **Configurable**: Fine-tune your setup through simple header files.
- **Extensible**: Built with SOLID and DRY principles in mind, making it easy to extend with new sensors and features.

## Installation

1.  **Clone the repository:**
    ```bash
    git clone https://github.com/TimothyFran/OpenMonitor.git
    cd OpenMonitor
    ```

2.  **Install PlatformIO:**
    If you don't have PlatformIO IDE installed, follow the instructions [here](https://platformio.org/install).

3.  **Create Configuration Files:**
    This project uses template files for configuration. You need to copy them to create your own configuration files.
    - `include/config.template.h` -> `include/config.h`
    - `include/settings.template.h` -> `include/settings.h`
    - `include/secrets.template.h` -> `include/secrets.h`

4.  **Build the project:**
    Open the project in your PlatformIO IDE (like VS Code with the PlatformIO extension) and build it. The required libraries will be downloaded automatically.

## Usage

Configuration is done through three main files in the `include/` directory. After copying the templates as described in the installation section, you need to edit them.

### `secrets.h`

This file is for your sensitive data. **Do not commit this file to version control.**

-   `SECRET_WIFI_SSID`: Your Wi-Fi network name.
-   `SECRET_WIFI_PASSWORD`: Your Wi-Fi password.
-   `SECRET_INFLUXDB_HOST`: The hostname or IP address of your InfluxDB instance.
-   `SECRET_INFLUXDB_BUCKET`: The InfluxDB bucket to write data to.
-   `SECRET_INFLUXDB_ORG`: Your InfluxDB organization.
-   `SECRET_INFLUXDB_TOKEN`: Your InfluxDB authentication token.

### `settings.h`

This file contains general settings for the application.

### `config.h`

This is where you enable and configure the sensors you want to use.

1.  **Enable the file:** Remove the `#if false` at the top of the file.
2.  **Include sensor headers:** Uncomment or add `#include` directives for the sensors you want to use (e.g., `#include <MPU6050Sensor.h>`).
3.  **Instantiate sensors:** Create instances of your sensor classes (e.g., `MPU6050Sensor mpu6050_1 = MPU6050Sensor("MPU6050_1");`).
4.  **Add sensors to the manager:** In the `addSensorsToManager()` function, add each sensor instance to the `sensorManager` (e.g., `sensorManager.addSensor(&mpu6050_1, true, true);`).

## Support

If you encounter any issues, feel free to open an [Issue](https://github.com/TimothyFran/OpenMonitor/issues) on the GitHub repository.

For commercial use requiring dedicated assistance (e.g., bug fixes, new sensor implementation, database integration other than InfluxDB), you can request a personalized consultation through my website: [www.franceschi.es](https://www.franceschi.es).

## Contributing

Contributions are what make the open source community such an amazing place to learn, inspire, and create. Any contributions you make are **greatly appreciated**.

If you have a suggestion that would make this better, please fork the repo and create a pull request. You can also simply open an issue with the tag "enhancement".
Don't forget to give the project a star! Thanks again!

1.  Fork the Project
2.  Create your Feature Branch (`git checkout -b feature/AmazingFeature`)
3.  Commit your Changes (`git commit -m 'Add some AmazingFeature'`)
4.  Push to the Branch (`git push origin feature/AmazingFeature`)
5.  Open a Pull Request

## Project Status

This project is currently a **hobby project**. It was created for fun, and the features I personally needed have already been implemented. I plan to add new sensors occasionally as I expand my own monitoring station. Development may be slow, but contributions are welcome!

## Authors

-   **Timothy Franceschi** - *Initial work* - [TimothyFran](https://github.com/TimothyFran)

## License

This project is licensed under the MIT License - see the `LICENSE` file for details.