 ```markdown
# InstagramTasker

InstagramTasker is a tool designed to bulk download and keep up-to-date Instagram profiles. It leverages the [InstaLoader](https://github.com/instaloader/instaloader/) library from GitHub for this purpose.

## Prerequisites

To use InstagramTasker, you need to have InstaLoader installed. You can install it using pip:

```sh
pip install instaloader
```

## Usage

InstagramTasker accepts a file as an argument where profiles should be inserted one per line. The program uses the `settings.txt` file for settings, which is based on the InstaLoader documentation. You are advised to create your own configuration file based on the [InstaLoader documentation](https://github.com/instaloader/instaloader#usage).

### Example Command

```sh
./InstagramTasker profiles.txt 1
```

- `profiles.txt`: The file containing Instagram profile names, one per line.
- `1`: The starting position for processing profiles.

## Features

- Bulk download of Instagram profiles.
- Keeps profiles up to date.
- Resumes from where it left off if interrupted.
- Configurable sleep time between profile downloads.
- Handles errors gracefully with retry and skip options.
- Added random profile read from list to avoid detection of bots (no constant same one by one profile download order)

## Building the Project

To build the project, you need CMake installed on your system. Follow these steps:

1. Clone the repository:
    ```sh
    git clone https://github.com/sapphirepro/InstagramTasker.git
    cd InstagramTasker
    ```

2. Create a build directory and navigate into it:
    ```sh
    mkdir build
    cd build
    ```

3. Run CMake to configure the project:
    ```sh
    cmake ..
    ```

4. Build the project using make:
    ```sh
    make
    ```

5. Run the executable:
    ```sh
    ./InstagramTasker
    ```

## Disclaimer

This program is meant for Linux (but with minor modifications can compile and run on any other OS as well). It is intended solely for personal use and for collecting your own important photos. The author has no liability for any misuse or abuse of this tool. Use it at your own risk; all warranties are void.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
```