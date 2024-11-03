#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>

void scanHosts(const std::string& filename, const std::string& scanType, const std::string& outputFile) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Erro ao abrir o arquivo: " << filename << std::endl;
        return;
    }

    std::string host;
    while (std::getline(file, host)) {
        if (!host.empty()) {
            // Monta o comando Nmap com o tipo de scan e saída
            std::string command = "nmap " + scanType + " -v " + host + " -oN " + outputFile;
            std::cout << "Executando: " << command << std::endl;
            // Executa o comando
            std::system(command.c_str());
        }
    }

    file.close();
}

int main(int argc, char* argv[]) {
    if (argc != 5) {
        std::cerr << "Uso: " << argv[0] << " <tipo_de_scan> <arquivo_de_hosts> -o <arquivo_de_saida>" << std::endl;
        std::cerr << "Tipos de scan: -s (silencioso), -a (agressivo)" << std::endl;
        return 1;
    }

    const std::string scanType = argv[1];
    const std::string filename = argv[2];
    const std::string outputOption = argv[3];
    const std::string outputFile = argv[4];

    // Verifica se a opção de saída é válida
    if (outputOption != "-o") {
        std::cerr << "Opção de saída inválida: " << outputOption << std::endl;
        return 1;
    }

    std::string nmapScanType;
    
    // Verifica se o tipo de scan é válido usando switch
    switch (scanType[1]) { // verifica o segundo caractere, pois o primeiro é '-'
        case 's':
            nmapScanType = "-sS -T2"; // Comando para o tipo de scan silencioso
            break;
        case 'a':
            nmapScanType = "-A"; // Comando para o tipo de scan agressivo
            break;
        default:
            std::cerr << "Tipo de scan inválido: " << scanType << std::endl;
            return 1;
    }

    scanHosts(filename, nmapScanType, outputFile);

    return 0;
}