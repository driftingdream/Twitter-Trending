#include <iostream>
#include <sstream>
#include <cstdlib>
#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Exception.hpp>
#include <curlpp/Options.hpp>
#include <nlohmann/json.hpp>
using json = nlohmann::json;
std::string GIST_ID = getenv("GIST_ID");
std::string GH_TOKEN = getenv("GH_TOKEN");

int main()
{
    std::string baseTwitterUrl = "https://api.twitter.com/1.1/trends/place.json?id=";
    std::list<std::string> headers;

    std::ostringstream os;
    curlpp::options::WriteStream ws(&os);

    using namespace curlpp::Options;
    using namespace std;
    std::string headerData = "Authorization: Bearer ";
    try
    {
        headerData.append(std::getenv("BEARER_TOKEN"));
        curlpp::Cleanup cleaner;
        curlpp::Easy request;
        std::list<std::string> headers;
        headers.push_back(headerData);
        request.setOpt(new HttpHeader(headers));
        request.setOpt(new Url(baseTwitterUrl));
        request.setOpt(ws);
        // request.setOpt(new Verbose(true));
        request.perform();


        string responseText = os.str();
        // cout << responseText << endl;
        json j = json::parse(responseText);
        string twitterTrend;
        for (int i = 0; i < j[0]["trends"].size(); i++)
        {
            twitterTrend += j[0]["trends"][i]["name"];
            twitterTrend += "\n";
        }
        twitterTrend.erase(twitterTrend.size() - 1, 1);

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