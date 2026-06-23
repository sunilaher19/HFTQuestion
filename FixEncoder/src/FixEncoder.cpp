#include <bits/stdc++.h>

using namespace std;

template<typename>
inline constexpr bool always_false_v = false;

constexpr char SOH = '|';

class FixEncoder{

    private:
        std::string header;
        std::string body;
        std::string trailer;

    public:

        FixEncoder()
        {
            body.reserve(1024);
            header.reserve(64);
            trailer.reserve(16);
        }

        void beginMessage(char msgType)
        {
            body = "35=" ;
            body += msgType;
        }

        template <typename T>
        void addField(int tag, const T& value)
        {
            body += SOH;
            Add(body, tag);
            body += '=';
            Add(body, value);
        }

        template <typename T>
        void Add(std::string& target,const T& value)
        {
            if constexpr (std::integral<T>)
            {
                char buffer[32];
                auto [ptr, ec] = to_chars(buffer, buffer + sizeof(buffer) , value);
                target.append(buffer,ptr); 
            }
            else if constexpr (std::floating_point<T>)
            {
                char buffer[32];
                auto [ptr, ec] = to_chars(buffer, buffer + sizeof(buffer) , value);
                target.append(buffer,ptr); 
            }
            else if constexpr (std::convertible_to<T, std::string_view> || std::convertible_to<T, std::string>)
            {
                target += value;
            }
            else
            {
                static_assert(always_false_v<T>,
                            "Unsupported FIX field type");
            }
            
        }

        void encode()
        {
            header = "8=FIX.4.4";
            header += SOH;
            header += "9=";
            Add(header, body.size());
            header += SOH;

            trailer  = SOH;
            trailer += "10=";
            int checksum=0;
            for(char c : header)
            {
                checksum += static_cast<unsigned char>(c);
            }
            for(char c : body)
            {
                checksum += static_cast<unsigned char>(c);
            }
            checksum %= 256;

            char chk[4];
            chk[0] = '0' + checksum / 100;
            chk[1] = '0' + (checksum / 10) % 10;
            chk[2] = '0' + checksum % 10;
            chk[3] = '\0';

            trailer.append(chk, chk+3);
        }

        void print()
        {
            encode();
            cout << header << body << trailer << endl;
        }
};


int main()
{
    FixEncoder fixEncoder;

    fixEncoder.beginMessage('e');
    fixEncoder.addField(11, "ORD123");
    fixEncoder.addField(55, "AAPL");
    fixEncoder.addField(54, 1);
    fixEncoder.addField(38, 100);
    fixEncoder.addField(44, 215.50);

    fixEncoder.print();

    return 0;
}