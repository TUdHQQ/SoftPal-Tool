#include <iostream>
#include <fstream>
#include <string>
#include <json/json.h>

using namespace std;

Json::Value readjson(const std::string &file) {
    std::ifstream jsonfile(file, std::ifstream::binary);
    Json::Value root;
    Json::CharReaderBuilder readerBuilder;
    std::string errs;

    if (!Json::parseFromStream(readerBuilder, jsonfile, &root, &errs)) {
        throw std::runtime_error("Failed to parse JSON file: " + file + ", error: " + errs);
    }

    return root;
}

void writejson(Json::Value &root, const std::string &file) {
    Json::StreamWriterBuilder builder;
    builder["emitUTF8"] = true;

    std::ofstream output(file);
    std::unique_ptr<Json::StreamWriter> writer(builder.newStreamWriter());
    writer->write(root, &output);
    output.close();
}

int main() {
    try {
        Json::Value inputJson = readjson("script_export.json");
        Json::Value outputJson(Json::arrayValue);
        for (const auto& item : inputJson) {
            Json::Value entry(Json::arrayValue);
            if (item["Name"].isNull()) {
                entry.append(Json::nullValue);
            } else {
                entry.append(item["Name"]["Translate"].asString());
            }
            entry.append(item["Text"]["Translate"].asString());
            outputJson.append(entry);
        }
        writejson(outputJson, "translate.json");      
    } catch (const std::exception &e) {
        cerr << e.what() << endl;
        return 1;
    }

    return 0;
}