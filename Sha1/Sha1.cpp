#include <sstream>
#include <iostream>
#include <cstdint>
#include <vector>
#include <bitset>
#include <iomanip>

std::string Sha_1(const std::string& input) 
{
	std::vector<uint8_t> message(input.begin(), input.end());
	std::vector<uint32_t> H = { 
		0x67452301, //H0
		0xEFCDAB89, //H1
		0x98BADCFE, //H2
		0x10325476, //H3
		0xC3D2E1F0  //H4
	};

	uint64_t len = message.size() * 8; // input data in bits

	message.push_back(0x80);	//adds byte to the end 

	while ((message.size() * 8) % 512 != 448) 
	{
		message.push_back(0x00); 
	}

	message.push_back(len >> 56);
	message.push_back(len >> 48);
	message.push_back(len >> 40);
	message.push_back(len >> 32);
	message.push_back(len >> 24);
	message.push_back(len >> 16);
	message.push_back(len >> 8);
	message.push_back(len);

	uint32_t A;
	uint32_t B;
	uint32_t C;
	uint32_t D;
	uint32_t E;

	for (uint64_t chunks = 0; chunks * 64 < message.size(); chunks++) 
	{
		std::vector<uint32_t> W(80);
		std::vector<uint32_t> M(16);

		for (int i = 0; i < 16; i++) 
		{
			M[i] = 0;

			for (int j = 0; j < 4; j++) 
			{
				M[i] = (M[i] << 8) | message[chunks * 64 + 4 * i + j];
			}
		}

		for (int i = 0; i < 16; i++) 
		{
			W[i] = M[i];
		}

		for (int i = 16; i < 80; i++) 
		{
			W[i] = (W[i - 3] ^ W[i - 8] ^ W[i - 14] ^ W[i - 16]);
			W[i] = (W[i] << 1) | (W[i] >> 31);
		}

		A = H[0];
		B = H[1];
		C = H[2];
		D = H[3];
		E = H[4];

		for (int i = 0; i < 80; i++) 
		{
			uint32_t Ft, Kt;

			if (i < 20) 
			{
				Ft = (B & C) | ((~B) & D);
				Kt = 0x5A827999;
			}
			else if (i < 40) 
			{
				Ft = B ^ C ^ D;
				Kt = 0x6ED9EBA1;
			}
			else if (i < 60) 
			{
				Ft = (B & C) | (B & D) | (C & D);
				Kt = 0x8F1BBCDC;
			}
			else 
			{
				Ft = B ^ C ^ D;
				Kt = 0xCA62C1D6;
			}

			uint32_t temp = ((A << 5) | (A >> 27)) + Ft + E + Kt + W[i];

			E = D;
			D = C;
			C = (B << 30) | (B >> 2); // shiftLeft
			B = A;
			A = temp;
		}

		H[0] += A;
		H[1] += B;
		H[2] += C;
		H[3] += D;
		H[4] += E;
	}

	std::stringstream hexstream;
	hexstream << std::hex << std::setfill('0');

	for (const auto& h : H) 
	{
		hexstream << std::setw(8) << h; 
	}

	std::string result(hexstream.str());

	return result;
}

int main() 
{
	std::string message = Sha_1("abcdef");

	std::cout << "SHA-1 - >    " << message << std::endl;

	return 0;
}