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
        Json::Value translateJson = readjson("translate.json");
        Json::Value script_exportJson = readjson("script_export.json");
        for (Json::ArrayIndex i = 0; i < translateJson.size(); ++i) {
            const Json::Value& entry = translateJson[i];
            if (entry[0].isNull()) {
                script_exportJson[i]["Name"] = Json::nullValue;
            } else {
                script_exportJson[i]["Name"]["Translate"] = entry[0].asString();
            }
            script_exportJson[i]["Text"]["Translate"] = entry[1].asString();
        }
        writejson(script_exportJson, "script_export.json");
    } catch (const std::exception &e) {
        cerr << e.what() << endl;
        return 1;
    }

    return 0;
}