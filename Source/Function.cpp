#include "Function.hpp"
#include <stdio.h>

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>

// For C++98, we'll use these typedefs
typedef unsigned int u32;
typedef unsigned char u8;

// Structure to store one function's metadata.
struct FunctionMetadata
{
    std::string mangled_name;
    u32 func_va;
    u32 func_fo;
    u32 og_addr;
    u32 func_status;
};

// A simple trim function to remove whitespace from the beginning and end of a string.
std::string trim(const std::string& s)
{
    std::string::size_type start = s.find_first_not_of(" \t\n\r");
    std::string::size_type end = s.find_last_not_of(" \t\n\r");
    if (start == std::string::npos)
        return "";
    return s.substr(start, end - start + 1);
}

// Converts a hex string (optionally starting with "0x" or "0X") into a u32.
u32 hexStringToU32(const std::string& hexStr)
{
    std::string s = trim(hexStr);
    if (s.size() >= 2 && (s.substr(0, 2) == "0x" || s.substr(0, 2) == "0X"))
        s = s.substr(2);
    return (u32)strtoul(s.c_str(), NULL, 16);
}

// Extracts the value of a given key (assumed to be in quotes) from a JSON object string.
std::string extractJsonStringValue(const std::string& objectStr, const std::string& key)
{
    std::string pattern = "\"" + key + "\"";
    std::string value = "";
    std::string::size_type keyPos = objectStr.find(pattern);
    if (keyPos == std::string::npos)
        return value;
    // Find the colon after the key.
    std::string::size_type colonPos = objectStr.find(":", keyPos);
    if (colonPos == std::string::npos)
        return value;
    // Find the starting quote for the value.
    std::string::size_type valueStart = objectStr.find("\"", colonPos);
    if (valueStart == std::string::npos)
        return value;
    ++valueStart; // Skip the quote.
    // Find the ending quote.
    std::string::size_type valueEnd = objectStr.find("\"", valueStart);
    if (valueEnd == std::string::npos)
        return value;
    value = objectStr.substr(valueStart, valueEnd - valueStart);
    return trim(value);
}

// Loads and parses the JSON from file "new_data.json", reading only the specific subset.
// It expects a key "functions" with an array of objects, where each object has keys:
// "mangled_name", "func_va", "func_fo", "og_addr", and "func_status".
std::vector<FunctionMetadata> loadFunctionMetadataFromFile(const std::string& filename)
{
    std::vector<FunctionMetadata> functions;
    FILE* fp = fopen(filename.c_str(), "rb");
    if (!fp)
    {
        printf("Error opening file: %s\n", filename.c_str());
        return functions;
    }

    // Determine the file size.
    fseek(fp, 0, SEEK_END);
    long fsize = ftell(fp);
    rewind(fp);

    // Read the entire file into a buffer.
    char* buffer = new char[fsize + 1];
    size_t readBytes = fread(buffer, 1, fsize, fp);
    buffer[readBytes] = '\0';
    fclose(fp);

    std::string fileContent(buffer);
    delete[] buffer;

    // Locate the "functions" key in the JSON.
    std::string::size_type functionsPos = fileContent.find("\"functions\"");
    if (functionsPos == std::string::npos)
    {
        printf("No \"functions\" key found.\n");
        return functions;
    }
    // Locate the start of the array '[' following the "functions" key.
    std::string::size_type arrayStart = fileContent.find("[", functionsPos);
    if (arrayStart == std::string::npos)
    {
        printf("No array '[' found after \"functions\" key.\n");
        return functions;
    }
    // Start searching from just after the '['.
    std::string::size_type pos = arrayStart + 1;

    // Loop through each JSON object in the array.
    while (true)
    {
        std::string::size_type objStart = fileContent.find("{", pos);
        if (objStart == std::string::npos)
            break;
        std::string::size_type objEnd = fileContent.find("}", objStart);
        if (objEnd == std::string::npos)
            break;

        // Extract the JSON object text.
        std::string objText = fileContent.substr(objStart, objEnd - objStart + 1);

        // Extract all required values.
        std::string mangledName = extractJsonStringValue(objText, "mangled_name");
        std::string funcVaStr = extractJsonStringValue(objText, "func_va");
        std::string funcFoStr = extractJsonStringValue(objText, "func_fo");
        std::string ogAddrStr = extractJsonStringValue(objText, "og_addr");
        std::string funcStatusStr = extractJsonStringValue(objText, "func_status");

        // If every field is non-empty, then convert the hex strings to u32 values and add the metadata.
        if (!mangledName.empty() && !funcVaStr.empty() && !funcFoStr.empty() && !ogAddrStr.empty() && !funcStatusStr.empty())
        {
            FunctionMetadata meta;
            meta.mangled_name = mangledName;
            meta.func_va = hexStringToU32(funcVaStr);
            meta.func_fo = hexStringToU32(funcFoStr);
            meta.og_addr = hexStringToU32(ogAddrStr);
            meta.func_status = hexStringToU32(funcStatusStr);
            functions.push_back(meta);
        }

        pos = objEnd + 1;
        std::string::size_type arrayEnd = fileContent.find("]", pos);
        if (arrayEnd != std::string::npos && pos > arrayEnd)
            break;
    }
    return functions;
}

// Searches backward from the given code address for a metadata signature.
// The assumed signature is: two NOPs (0x90, 0x90) followed by a MOV EAX immediate opcode (0xB8).
// If found, returns the address (as u32) where the pattern starts; otherwise returns 0.
static u32 find_func_meta_data(u32 codeAddr)
{
    // Search backwards up to 10 bytes.
    for (u32 offset = 1; offset <= 100; ++offset)
    {
        u32 candidateAddr = codeAddr - offset;
        // Cast the candidate address into a pointer to bytes.
        const u8* candidate = reinterpret_cast<const u8*>(candidateAddr);
        // Check for the pattern.
        // (Note: Make sure that reading these 3 bytes is safe in your environment.)
        if (candidate[0] == 0x90 && candidate[1] == 0x90 && candidate[2] == 0xB8)
        {
            return (candidate[3]) | (candidate[4] << 8) | (candidate[5] << 16) | (candidate[6] << 24);
        }
    }
    return 0; // Not found.
}

void __stdcall LogNotImplemented(u32 codeAddr)
{
    const u32 ogAddr = find_func_meta_data(codeAddr);
    if (ogAddr == 0)
    {
        printf("CANT FIND OG ADDR!!!! 0x%X\n", codeAddr);
    }
    else
    {
        static std::vector<FunctionMetadata> functions = loadFunctionMetadataFromFile("new_data.json");
        for (unsigned int i = 0; i < functions.size(); ++i)
        {
            if (functions[i].og_addr == ogAddr)
            {
                printf("NOT IMPLEMENTED: %s (0x%X)\n", functions[i].mangled_name.c_str(), ogAddr);
                return;
            }
        }
        printf("NOT IMPLEMENTED: (???) 0x%X\n", ogAddr);
    }
}