# V2X Application Spoofing Platform (VASP) 🦟

This repository provides a framework to simulate attacks on V2X networks. It uses the [VEINS](https://veins.car2x.org/) simulation framework as a dependency.

# Dependencies

1. Operating System: Ubuntu/Debian/macOS
2. [CSVWriter.h](https://github.com/al-eax/CSVWriter/blob/cee5f9d0ec72120404c1510708ba818307a6ab80/include/CSVWriter.h)
    * Please put this file in your system's (or user's) default include directory. E.g., `/usr/include`
3. [json.hpp](https://github.com/nlohmann/json/releases/download/v3.10.5/json.hpp)
    * Rename this file to `json.h` and
    * Put it in your system's (or user's) default include directory. E.g., `/usr/include`

# Installation

1. Download all of the following into the same folder (preferably called "src")
2. Download and extract [OMNeT++](https://omnetpp.org/) (version 5.6.2)
    * Please uncomment the line with `CXXFLAGS` in the "configure.user" file in the root directory of OMNeT++ with `CXXFLAGS=-std=c++14`
    * Install by following their [installation guide](https://doc.omnetpp.org/omnetpp/InstallGuide.pdf).
3. Install [SUMO](https://www.eclipse.org/sumo/) (version 1.8.0) by following their [installation guide](https://sumo.dlr.de/docs/Installing/index.html).
4. Clone [VEINS](https://veins.car2x.org/) (version 5.2): `git clone --branch veins-5.2 https://github.com/sommer/veins.git`
5. Add this repo as submodule under `<path/to/veins>/src/`: `cd veins && git submodule add https://github.qualcomm.com/cavalry/vasp src/vasp`

# Build

1. Change directory to `<path/to/veins>`
2. Configure and build
    ```sh
    ./configure && make [-j6]
    ```

# Running simulations
1. Change directory to `<path/to/veins>`
2. Start `sumo` server: `bin/veins_launchd -vv`
3. Change directory to `<path/to/veins>/src/vasp/scenario/`
4. Run simulation:
    ```sh
    ./run [-u Cmdenv]
    ```
5. You should see a trace file generated under `<path/to/veins>/src/vasp/scenario/results` folder.

# More Documentation
1. [Configuring your simulation](docs/configuring_simulations.md)
2. [Know your trace file](docs/trace_file_column_explanation.md)
3. [Implementing your own attack](docs/implement_attack.md)

# License
MIT