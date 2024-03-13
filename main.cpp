#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>

using namespace std;

// �ļ�������
class FileManager {
private:
    ifstream originalFile;  // ԭ���ļ���
    ifstream plagiarizedFile;  // ��Ϯ���ļ���
    ofstream outputFile;  // ����ļ���

public:
    // ���캯������ԭ�ġ���Ϯ�������ļ�
    FileManager(const string& originalFileName, const string& plagiarizedFileName, const string& outputFileName) {
        originalFile.open(originalFileName);
        plagiarizedFile.open(plagiarizedFileName);
        outputFile.open(outputFileName);
    }

    // �����������ر��ļ���
    ~FileManager() {
        originalFile.close();
        plagiarizedFile.close();
        outputFile.close();
    }

    // ����ļ��Ƿ�ɹ���
    bool isOpen() const {
        return originalFile.is_open() && plagiarizedFile.is_open() && outputFile.is_open();
    }

    // ��ȡԭ���ļ���
    ifstream& getOriginalFile() {
        return originalFile;
    }

    // ��ȡ��Ϯ���ļ���
    ifstream& getPlagiarizedFile() {
        return plagiarizedFile;
    }

    // ��ȡ����ļ���
    ofstream& getOutputFile() {
        return outputFile;
    }
};

// �ı�������
class TextProcessor {
public:
    // ��ϴ�ı���ȥ�������ţ�ת��ΪСд
    static string cleanText(const string& text) {
        string cleanedText;
        for (char ch : text) {
            if (isalpha(ch) || ch == ' ') {
                cleanedText += tolower(ch);
            }
        }
        return cleanedText;
    }

    // �������ƶȣ�ͳ����ͬ��������������ٷֱ�
    static double calculateSimilarity(const string& originalText, const string& plagiarizedText) {
        string cleanedOriginal = cleanText(originalText);  // ��ϴԭ��
        string cleanedPlagiarized = cleanText(plagiarizedText);  // ��ϴ��Ϯ��

        int matchCount = 0;  // ƥ�䵥������
        int totalWords = 0;  // �ܵ�������

        size_t startPos = 0;
        size_t foundPos = 0;
        while ((foundPos = cleanedPlagiarized.find(' ', startPos)) != string::npos) {
            string word = cleanedPlagiarized.substr(startPos, foundPos - startPos);
            totalWords++;  // ͳ���ܵ�������
            if (cleanedOriginal.find(word) != string::npos) {
                matchCount++;  // ƥ�䵥��������һ
            }
            startPos = foundPos + 1;
        }

        if (startPos < cleanedPlagiarized.length()) {
            string word = cleanedPlagiarized.substr(startPos);
            totalWords++;  // ͳ���ܵ�������
            if (cleanedOriginal.find(word) != string::npos) {
                matchCount++;  // ƥ�䵥��������һ
            }
        }

        return static_cast<double>(matchCount) / totalWords * 100;  // �������ƶȰٷֱ�
    }
};

int main() {
    string originalFileName = "org.txt";  // ԭ���ļ���
    string plagiarizedFileName = "addorg.txt";  // ��Ϯ���ļ���
    string outputFileName = "ans.txt";  // ����ļ���

    // �����ļ������������ļ�
    FileManager fileManager(originalFileName, plagiarizedFileName, outputFileName);
    if (!fileManager.isOpen()) {
        cerr << "Error: Unable to open files!" << endl;
        return 1;
    }

    // ��ȡ�ļ���
    ifstream& originalFile = fileManager.getOriginalFile();
    ifstream& plagiarizedFile = fileManager.getPlagiarizedFile();
    ofstream& outputFile = fileManager.getOutputFile();

    // ��ȡԭ�ĺͳ�Ϯ���ı�����
    string originalText((istreambuf_iterator<char>(originalFile)), istreambuf_iterator<char>());
    string plagiarizedText((istreambuf_iterator<char>(plagiarizedFile)), istreambuf_iterator<char>());

    // �������ƶ�
    double similarity = TextProcessor::calculateSimilarity(originalText, plagiarizedText);

    // �����ƶ�д������ļ�
    outputFile << fixed << setprecision(2) << similarity;

    cout << "Similarity calculated and written to the output file successfully!" << endl;

    return 0;
}
