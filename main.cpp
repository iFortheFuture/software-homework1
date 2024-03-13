#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>

using namespace std;

// 文件管理类
class FileManager {
private:
    ifstream originalFile;  // 原文文件流
    ifstream plagiarizedFile;  // 抄袭版文件流
    ofstream outputFile;  // 输出文件流

public:
    // 构造函数：打开原文、抄袭版和输出文件
    FileManager(const string& originalFileName, const string& plagiarizedFileName, const string& outputFileName) {
        originalFile.open(originalFileName);
        plagiarizedFile.open(plagiarizedFileName);
        outputFile.open(outputFileName);
    }

    // 析构函数：关闭文件流
    ~FileManager() {
        originalFile.close();
        plagiarizedFile.close();
        outputFile.close();
    }

    // 检查文件是否成功打开
    bool isOpen() const {
        return originalFile.is_open() && plagiarizedFile.is_open() && outputFile.is_open();
    }

    // 获取原文文件流
    ifstream& getOriginalFile() {
        return originalFile;
    }

    // 获取抄袭版文件流
    ifstream& getPlagiarizedFile() {
        return plagiarizedFile;
    }

    // 获取输出文件流
    ofstream& getOutputFile() {
        return outputFile;
    }
};

// 文本处理类
class TextProcessor {
public:
    // 清洗文本：去除标点符号，转换为小写
    static string cleanText(const string& text) {
        string cleanedText;
        for (char ch : text) {
            if (isalpha(ch) || ch == ' ') {
                cleanedText += tolower(ch);
            }
        }
        return cleanedText;
    }

    // 计算相似度：统计相同单词数量，计算百分比
    static double calculateSimilarity(const string& originalText, const string& plagiarizedText) {
        string cleanedOriginal = cleanText(originalText);  // 清洗原文
        string cleanedPlagiarized = cleanText(plagiarizedText);  // 清洗抄袭版

        int matchCount = 0;  // 匹配单词数量
        int totalWords = 0;  // 总单词数量

        size_t startPos = 0;
        size_t foundPos = 0;
        while ((foundPos = cleanedPlagiarized.find(' ', startPos)) != string::npos) {
            string word = cleanedPlagiarized.substr(startPos, foundPos - startPos);
            totalWords++;  // 统计总单词数量
            if (cleanedOriginal.find(word) != string::npos) {
                matchCount++;  // 匹配单词数量加一
            }
            startPos = foundPos + 1;
        }

        if (startPos < cleanedPlagiarized.length()) {
            string word = cleanedPlagiarized.substr(startPos);
            totalWords++;  // 统计总单词数量
            if (cleanedOriginal.find(word) != string::npos) {
                matchCount++;  // 匹配单词数量加一
            }
        }

        return static_cast<double>(matchCount) / totalWords * 100;  // 计算相似度百分比
    }
};

int main() {
    string originalFileName = "org.txt";  // 原文文件名
    string plagiarizedFileName = "addorg.txt";  // 抄袭版文件名
    string outputFileName = "ans.txt";  // 输出文件名

    // 创建文件管理器，打开文件
    FileManager fileManager(originalFileName, plagiarizedFileName, outputFileName);
    if (!fileManager.isOpen()) {
        cerr << "Error: Unable to open files!" << endl;
        return 1;
    }

    // 获取文件流
    ifstream& originalFile = fileManager.getOriginalFile();
    ifstream& plagiarizedFile = fileManager.getPlagiarizedFile();
    ofstream& outputFile = fileManager.getOutputFile();

    // 读取原文和抄袭版文本内容
    string originalText((istreambuf_iterator<char>(originalFile)), istreambuf_iterator<char>());
    string plagiarizedText((istreambuf_iterator<char>(plagiarizedFile)), istreambuf_iterator<char>());

    // 计算相似度
    double similarity = TextProcessor::calculateSimilarity(originalText, plagiarizedText);

    // 将相似度写入输出文件
    outputFile << fixed << setprecision(2) << similarity;

    cout << "Similarity calculated and written to the output file successfully!" << endl;

    return 0;
}
