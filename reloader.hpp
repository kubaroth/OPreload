#pragma once

#include <glob.h> // glob(), globfree()
#include <string.h> // memset()
#include <vector>
#include <stdexcept>
#include <string>
#include <sstream>

#include <regex>
#include <iomanip>  // setw

#include <UT/UT_FileUtil.h>  // copy file

OP_OperatorTable *g_table;
OP_Operator *g_op;

// https://stackoverflow.com/questions/1008019/c-singleton-design-pattern
class VersionCounter
{
public:
    static VersionCounter& getInstance() {
        static VersionCounter    instance; // Guaranteed to be destroyed.
                                           // Instantiated on first use.
        return instance;
    }

private:
    VersionCounter() {}

public:
    VersionCounter(VersionCounter const&)  = delete;
    void operator=(VersionCounter const&)  = delete;
    // Note: Scott Meyers mentions in his Effective Modern
    //       C++ book, that deleted functions should generally
    //       be public as it results in better error messages
    //       due to the compilers behavior to check accessibility
    //       before deleted status

    int dso_version = 0;
};


//https://stackoverflow.com/questions/8401777/simple-glob-in-c-on-unix-system
std::vector<std::string> glob(const std::string& pattern) {
    using namespace std;

    // glob struct resides on the stack
    glob_t glob_result;
    memset(&glob_result, 0, sizeof(glob_result));

    // do the glob operation
    int return_value = glob(pattern.c_str(), GLOB_TILDE, NULL, &glob_result);
    if(return_value != 0) {
        globfree(&glob_result);
        stringstream ss;
        ss << "glob() failed with return_value " << return_value << endl;
        throw std::runtime_error(ss.str());
    }

    // collect all the filenames into a std::list<std::string>
    vector<string> filenames;
    for(size_t i = 0; i < glob_result.gl_pathc; ++i) {
        filenames.push_back(string(glob_result.gl_pathv[i]));
    }

    // cleanup
    globfree(&glob_result);

    // done
    return filenames;
}

OP_ERROR reload_callback(std::basic_ostream<char>& a, void* b){

    using std::cout;
    using std::endl;

    // Increment dso file - copy the original
    std::string directory = PLUGIN_DIR;
    std::string base_name = "libhoudini_reload"; // TODO replace with regex
    
    auto list = glob(directory + '/' + "*.so");
    int total_files = list.size();

    cout << "built file: " << list[0] << endl; // first one in the list

    // TODO:
    // /// Extract base name
    // std::smatch m;
    // // get full path any file with the same base name we are after
    // std::string s = list[0];
    // std::regex str_expr ("[a-z_A-Z]{3,}");

    // // base file name is the last match (longer then 3 characters)
    // std::string base_name1;
    
    // while (std::regex_search (s, m, str_expr)){
    //     base_name1 = m[0];
    //     cout << base_name1 <<endl;
    //     s = m.suffix().str();
    // }
    // cout << "_______"<< base_name1 <<endl;
    
    // increment version and add the padding
    std::stringstream ss;
    ss << std::setfill('0') << std::setw(5) << std::to_string(total_files + 1);

    std::string dest = directory + '/' + base_name + ss.str() + ".so";
    cout << "copy to: " << dest <<endl;

    // Copy
    UT_FileUtil::copyFile(list[0].c_str(), dest.c_str());

    // Update plugin
    g_table->removeOperator(g_op);
    g_table->requestReload();
    
    cout << "loading dso: "<< dest <<endl;;
    g_table->loadDSO(dest.c_str());
        
    return UT_ERROR_NONE;
}
