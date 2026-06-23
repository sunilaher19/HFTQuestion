#include <bits/stdc++.h>

using namespace std;

class FIXParser{

    private:
        unordered_map<int, std::string_view> fixParserMap;
        std::string m_buffer;


    public:

        void clear() { fixParserMap.clear(); }

        bool hasField(int tag) const{
            return fixParserMap.find(tag) != fixParserMap.end();
        }

        std::string_view getField(int tag) const
        {
            auto it = fixParserMap.find(tag);

            return it == fixParserMap.end()
                        ? std::string_view{}
                        : it->second;
        }
        
        void parse(std::string_view msg)
        {
            m_buffer.assign(msg);
            size_t pos = 0;

            while(pos < msg.size())
            {
                auto pipePos = msg.find('|', pos);

                if(pipePos == std::string_view::npos)
                    break;

                auto equalPos = msg.find('=', pos);

                if(equalPos == std::string_view::npos ||
                equalPos > pipePos)
                    break;

                auto tag = msg.substr(pos, equalPos - pos);

                auto value = msg.substr(
                    equalPos + 1,
                    pipePos - equalPos - 1
                );

                fixParserMap[std::stoi(std::string(tag))] = value;

                pos = pipePos + 1;
        }
    }

};

int main()
{
    FIXParser parser;

    parser.parse("35=D|55=AAPL|44=210.50|38=100|");

    return 0;
}