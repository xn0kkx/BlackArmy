#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>

void scanHosts(const std::string& filename, const std::string& scanType, const std::string& outputFile) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return;
    }

    std::string host;
    while (std::getline(file, host)) {
        if (!host.empty()) {
            // Build the Nmap command with the scan type and output
            std::string command = "nmap " + scanType + " -v " + host + " -oN " + outputFile;
            std::cout << "Executing: " << command << std::endl;
            // Execute the command
            std::system(command.c_str());
        }
    }

    file.close();
}

int main(int argc, char* argv[]) {
    if (argc != 5) {
        std::cerr << "Usage: " << argv[0] << " <scan_type> <hosts_file> -o <output_file>" << std::endl;
        std::cerr << "Scan types: -s (silent), -a (aggressive), -sv (version)" << std::endl;
        return 1;
    }

    const std::string scanType = argv[1];
    const std::string filename = argv[2];
    const std::string outputOption = argv[3];
    const std::string outputFile = argv[4];

    // Check if the output option is valid
    if (outputOption != "-o") {
        std::cerr << "Invalid output option: " << outputOption << std::endl;
        return 1;
    }

    std::string nmapScanType;

    // Check if the scan type is valid using switch
    switch (scanType[1]) { // check the second character, as the first is '-'
        case 's':
            nmapScanType = "-sS -T2"; // Command for silent scan type
            break;
        case 'a':
            nmapScanType = "-A"; // Command for aggressive scan type
            break;
        case 'v':
            nmapScanType = "-sV"; // Command for version scan type
            break;
        default:
            std::cerr << "Invalid scan type: " << scanType << std::endl;
            return 1;
    }

    scanHosts(filename, nmapScanType, outputFile);

    return 0;
}