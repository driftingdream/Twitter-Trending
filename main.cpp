#include <iostream>
#include <sstream>
#include <cstdlib>
#include <thread>
#include <chrono>
#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Exception.hpp>
#include <curlpp/Options.hpp>
#include <nlohmann/json.hpp>
using json = nlohmann::json;
std::string GIST_ID = getenv("GIST_ID");
std::string GH_TOKEN = getenv("GH_TOKEN");

class Dict
{
private:
    std::string country, countryID;

public:
    Dict(std::string country, std::string countryID);
    ~Dict();
    std::string getCountry() const
    {
        return this->country;
    }
    std::string getCountryID() const
    {
        return this->countryID;
    }
};

Dict::Dict(std::string country, std::string countryID)
{
    this->country = country;
    this->countryID = countryID;
}

Dict::~Dict()
{
}
Dict worldDict[] = {
    Dict("USA", "23424977"),
    Dict("UK", "23424975"),
    Dict("World", "1"),
    Dict("Belarus", "23424765"),
    Dict("Australia", "23424748")};

int length = sizeof(worldDict) / sizeof(worldDict[0]);

void updateGist(std::string fileName, std::string &content)
{
    using namespace curlpp::options;
    std::list<std::string> headers;
    std::string gistUrl = "https://api.github.com/gists/";
    gistUrl += GIST_ID;
    headers.push_back("Content-Type: application/json");
    std::string auth = "Authorization: token ";
    auth.append(GH_TOKEN);
    headers.push_back(auth);
    headers.push_back("User-Agent: twitter_trend");
    curlpp::Easy requests;
    json jsonData = R"(
        {
            "files":
            {

            }
        }
    )"_json;
    jsonData["files"][fileName]["content"] = content;
    std::string postData = jsonData.dump();
    std::ostringstream os;
    requests.setOpt(new WriteStream(&os));
    // requests.setOpt(new Verbose(true));
    requests.setOpt(new Url(gistUrl));
    requests.setOpt(new SslEngineDefault());
    requests.setOpt(new HttpHeader(headers));
    requests.setOpt(new PostFields(postData));
    requests.setOpt(new PostFieldSize(postData.length()));
    requests.perform();
}

int main()
{
    std::string baseTwitterUrl = "https://api.twitter.com/1.1/trends/place.json?id=";
    std::stringstream fileNameStream;
    std::string fileName;

    using namespace curlpp::Options;
    using namespace std;
    try
    {
        for (int i = 0; i < length; i++)
        {
            std::list<std::string> headers;
            std::ostringstream os;
            curlpp::options::WriteStream ws(&os);
            std::string headerData = "Authorization: Bearer ";
            std::string requestUrl = baseTwitterUrl + worldDict[i].getCountryID();
            fileNameStream << "Twitter Trending(" << worldDict[i].getCountry() << ").txt";
            fileName = fileNameStream.str();
            fileNameStream.clear();
            fileNameStream.str("");
            headerData.append(std::getenv("BEARER_TOKEN"));
            curlpp::Cleanup cleaner;
            curlpp::Easy request;
            headers.push_back(headerData);
            request.setOpt(new HttpHeader(headers));
            request.setOpt(new Url(requestUrl));
            request.setOpt(ws);
            // request.setOpt(new Verbose(true));
            request.perform();

            string responseText = os.str();
            // cout << i << endl;
            // cout << responseText << endl;
            json j = json::parse(responseText);
            string twitterTrend;
            for (int i = 0; i < j[0]["trends"].size(); i++)
            {
                twitterTrend += to_string(i + 1) + ' ';
                twitterTrend += j[0]["trends"][i]["name"];
                twitterTrend += "    ";
                twitterTrend += to_string(j[0]["trends"][i]["tweet_volume"]);
                twitterTrend += "\n";
            }
            twitterTrend.erase(twitterTrend.size() - 1, 1);
            // cout << twitterTrend << endl;
            updateGist(fileName, twitterTrend);
            // std::this_thread::sleep_for(std::chrono::seconds(3));
        }
    }
    catch (curlpp::LogicError &e)
    {
        std::cout << "LogicError: ";
        std::cout << e.what() << std::endl;
    }
    catch (curlpp::RuntimeError &e)
    {
        std::cout << "RuntimeError:";
        std::cout << e.what() << std::endl;
    }
    catch (exception &e)
    {
        std::cout << e.what() << endl;
    }
    return 0;
}