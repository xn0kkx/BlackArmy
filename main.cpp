#include <iostream>
#include <fstream>
#include <cstdio>
#include <memory>
#include <stdexcept>
#include <string>

// Função para rodar o Nmap em um IP específico
std::string runNmap(const std::string& ipAddress, const std::string& nmapCommand) {
    std::string command = "nmap " + nmapCommand + " " + ipAddress;
    std::string result;

    // Abre um pipe para o comando
    std::shared_ptr<FILE> pipe(popen(command.c_str(), "r"), pclose);
    if (!pipe) throw std::runtime_error("Erro ao abrir o pipe!");

    // Lê a saída do comando e armazena em `result`
    char buffer[128];
    while (fgets(buffer, sizeof(buffer), pipe.get()) != nullptr) {
        result += buffer;
    }

    return result;
}

// Função para rodar o Amass em um IP ou hostname específico
std::string runAmass(const std::string& target, const std::string& amassCommand) {
    std::string command = "amass " + amassCommand + " -d " + target;
    std::string result;

    // Abre um pipe para o comando
    std::shared_ptr<FILE> pipe(popen(command.c_str(), "r"), pclose);
    if (!pipe) throw std::runtime_error("Erro ao abrir o pipe!");

    // Lê a saída do comando e armazena em `result`
    char buffer[128];
    while (fgets(buffer, sizeof(buffer), pipe.get()) != nullptr) {
        result += buffer;
    }

    return result;
}

// Função para rodar o Nuclei em um IP ou hostname específico
std::string runNuclei(const std::string& target, const std::string& nucleiCommand) {
    std::string command = "nuclei -u " + target + " " + nucleiCommand;
    std::string result;

    // Abre um pipe para o comando
    std::shared_ptr<FILE> pipe(popen(command.c_str(), "r"), pclose);
    if (!pipe) throw std::runtime_error("Erro ao abrir o pipe!");

    // Lê a saída do comando e armazena em `result`
    char buffer[128];
    while (fgets(buffer, sizeof(buffer), pipe.get()) != nullptr) {
        result += buffer;
    }

    return result;
}

int main(int argc, char* argv[]) {
    // Verifica se o usuário forneceu o arquivo de alvos e o tipo de scan
    if (argc != 3) {
        std::cerr << "Uso: " << argv[0] << " <arquivo_alvos.txt> <tipo_scan>\n";
        std::cerr << "Tipos de scan:\n"
                  << "1 - Scan básico\n"
                  << "2 - Detecção de versão\n"
                  << "3 - Detecção de sistema operacional\n"
                  << "4 - Scan agressivo\n";
        return 1;
    }

    // Abre o arquivo de alvos
    std::ifstream alvoFile(argv[1]);
    if (!alvoFile) {
        std::cerr << "Erro ao abrir o arquivo de alvos: " << argv[1] << std::endl;
        return 1;
    }

    // Define o tipo de comando Nmap, Amass e Nuclei com base no argumento
    std::string nmapCommand;
    std::string amassCommand;
    std::string nucleiCommand;
    int scanType = std::stoi(argv[2]);  // Converte o argumento para um inteiro

    switch (scanType) {
        case 1:
            nmapCommand = "-sP";             // Scan básico (ping scan)
            amassCommand = "enum";            // Enumeração básica de subdomínios com Amass
            nucleiCommand = "-tags cves";     // Scan básico com Nuclei focado em CVEs
            break;
        case 2:
            nmapCommand = "-sV";              // Detecção de versão de serviços
            amassCommand = "enum -active";    // Enumeração ativa com Amass
            nucleiCommand = "-tags exposures";// Scan de exposições com Nuclei
            break;
        case 3:
            nmapCommand = "-O";               // Detecção de sistema operacional
            amassCommand = "intel";           // Amass modo de inteligência para coleta de informações
            nucleiCommand = "-tags misconfig"; // Scan de misconfigurações com Nuclei
            break;
        case 4:
            nmapCommand = "-A";               // Scan agressivo
            amassCommand = "viz -d3";         // Visualização de resultados com Amass
            nucleiCommand = "-tags default";  // Scan agressivo padrão com Nuclei
            break;
        default:
            std::cerr << "Tipo de scan inválido! Escolha entre 1, 2, 3 ou 4.\n";
            return 1;
    }

    // Lê e escaneia cada alvo no arquivo com Nmap, Amass e Nuclei
    std::string ipAddress;
    while (std::getline(alvoFile, ipAddress)) {
        if (ipAddress.empty()) continue;  // Pula linhas vazias

        // Executa o scan Nmap
        std::cout << "Escaneando " << ipAddress << " com o comando Nmap: " << nmapCommand << "...\n";
        try {
            std::string nmapResult = runNmap(ipAddress, nmapCommand);
            std::cout << "Resultado do escaneamento Nmap para " << ipAddress << ":\n"
                      << nmapResult << "\n" << std::endl;
        } catch (const std::exception& e) {
            std::cerr << "Erro ao escanear " << ipAddress << " com Nmap: " << e.what() << std::endl;
        }

        // Executa o scan Amass
        std::cout << "Escaneando " << ipAddress << " com o comando Amass: " << amassCommand << "...\n";
        try {
            std::string amassResult = runAmass(ipAddress, amassCommand);
            std::cout << "Resultado do escaneamento Amass para " << ipAddress << ":\n"
                      << amassResult << "\n" << std::endl;
        } catch (const std::exception& e) {
            std::cerr << "Erro ao escanear " << ipAddress << " com Amass: " << e.what() << std::endl;
        }

        // Executa o scan Nuclei
        std::cout << "Escaneando " << ipAddress << " com o comando Nuclei: " << nucleiCommand << "...\n";
        try {
            std::string nucleiResult = runNuclei(ipAddress, nucleiCommand);
            std::cout << "Resultado do escaneamento Nuclei para " << ipAddress << ":\n"
                      << nucleiResult << "\n" << std::endl;
        } catch (const std::exception& e) {
            std::cerr << "Erro ao escanear " << ipAddress << " com Nuclei: " << e.what() << std::endl;
        }
    }

    return 0;
}