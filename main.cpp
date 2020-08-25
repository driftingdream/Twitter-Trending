#include <iostream>
#include <sstream>
#include <cstdlib>
#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Exception.hpp>
#include <curlpp/Options.hpp>

int main()
{
    std::string twitterUrl = "https://api.twitter.com/1.1/trends/place.json?id=23424977";
    std::list<std::string> headers;

    std::ostringstream os;
    curlpp::options::WriteStream ws(&os);

    std::string headerData = "Authorization: Bearer ";
    headerData.append(std::getenv("BEARER_TOKEN"));
    using namespace curlpp::Options;
    using namespace std;
    try
    {
        curlpp::Cleanup cleaner;
        curlpp::Easy request;
        std::list<std::string> headers;
        headers.push_back(headerData);
        request.setOpt(new HttpHeader(headers));
        request.setOpt(new Url(twitterUrl));
        request.setOpt(ws);
        request.setOpt(new Verbose(true));
        request.perform();
        string responseText = os.str();
        cout << responseText;

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