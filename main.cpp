#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <cstdio>

void runGobuster(const std::string& domain, const std::string& wordlist, const std::string& outputFile) {
    // Build the Gobuster command with wordlist and domain, appending output to file
    std::string command = "gobuster dns -d " + domain + " -w " + wordlist + " >> " + outputFile + " 2>&1"; // Redirect errors as well
    std::cout << "Executing Gobuster: " << command << std::endl;
    // Execute the Gobuster command
    std::system(command.c_str());
}

void runAmass(const std::string& domain, const std::string& outputFile) {
    // Build the Amass command to run
    std::string command = "amass enum -d " + domain + " -o " + outputFile + " >> " + outputFile + " 2>&1"; // Append to output file
    std::cout << "Executing Amass: " << command << std::endl;
    // Execute the Amass command
    std::system(command.c_str());
}

void scanHosts(const std::string& filename, const std::string& scanType, const std::string& outputFile) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return;
    }

    std::string host;
    while (std::getline(file, host)) {
        if (!host.empty()) {
            // Build the Nmap command with the scan type and append output
            std::string command = "nmap " + scanType + " -v " + host + " >> " + outputFile; // Append output with >>
            std::cout << "Executing Nmap: " << command << std::endl;
            // Execute the Nmap command
            std::system(command.c_str());
        }
    }

    file.close();
}

int main(int argc, char* argv[]) {
    if (argc != 7) {
        std::cerr << "Usage: " << argv[0] << " <scan_type> <hosts_file> -o <output_file> -w <wordlist>" << std::endl;
        std::cerr << "Scan types: -s (silent), -a (aggressive), -sv (version)" << std::endl;
        return 1;
    }

    const std::string scanType = argv[1];
    const std::string filename = argv[2];
    const std::string outputOption = argv[3];
    const std::string outputFile = argv[4];
    const std::string wordlistOption = argv[5];
    const std::string wordlist = argv[6];

    // Remove the output file if it exists to ensure a fresh file each run
    std::remove(outputFile.c_str());

    // Check if the output option is valid
    if (outputOption != "-o") {
        std::cerr << "Invalid output option: " << outputOption << std::endl;
        return 1;
    }

    // Check if the wordlist option is valid
    if (wordlistOption != "-w") {
        std::cerr << "Invalid wordlist option: " << wordlistOption << std::endl;
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

    // Run Gobuster and Amass enumeration before the Nmap scan
    // Read the first line of the hosts file to get the domain
    std::ifstream hostFile(filename);
    std::string domain;
    if (std::getline(hostFile, domain)) {
        runGobuster(domain, wordlist, outputFile); // Run Gobuster with the domain and wordlist
        runAmass(domain, outputFile); // Run Amass with the domain
    } else {
        std::cerr << "No domain found in the hosts file: " << filename << std::endl;
        return 1;
    }
    hostFile.close();

    // Proceed to scan hosts with Nmap
    scanHosts(filename, nmapScanType, outputFile);

    return 0;
}