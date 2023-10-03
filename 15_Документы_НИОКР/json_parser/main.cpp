#include "main.h"

int main(int argc, char *argv[])
{

    const path archiv_path{ R"(/home/evg/Desktop/temp_arсhive)" };
    const unordered_set<string> pe_extensions{ ".json" };
    cout << endl <<"Patch for json search: "<<archiv_path << endl <<endl;
    vector<string> vectorJsonPatch;

    auto iterator = recursive_directory_iterator{ archiv_path, directory_options::skip_permission_denied };
    for(const auto& entry : iterator) {
      try {
        if(!entry.is_regular_file())
          continue;
        const auto& extension = entry.path().extension().string();
        const auto is_pe = pe_extensions.find(extension) != pe_extensions.end();
        if(!is_pe)
          continue;
        ifstream file{ entry.path() };
        string patch = entry.path().string();
        vectorJsonPatch.push_back(patch);
      } catch(const exception& e) {
        cerr << "Error reading " << entry.path().string() << ": " << e.what() << endl;
      }
    }

    if (vectorJsonPatch.size()>0)
    {
        int num = 0;
        for (auto & patchJson : vectorJsonPatch){
            ++num;
            cout << endl<< num << ": "<< patchJson << endl;
            parseJSON(patchJson);
        }
    }

}
