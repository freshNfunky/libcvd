#include "cvd/config.h"
#include "cvd/image_convert.h"
#include "cvd/utility.h"

namespace CVD 
{
	namespace Internal
	{
		extern "C"{
			void cvd_asm_rgb_to_gray(const unsigned char* in, unsigned char* out, int size, int wr, int wg, int wb);
		}
	}

    void ConvertImage<Rgb<byte>, byte, Pixel::CIE<Rgb<byte>, byte>, 1>::convert(const BasicImage<Rgb<byte> >& from, BasicImage<byte>& to) 
	{
		const Rgb<byte>* rgb = from.data();
		byte* gray = to.data();
        int count=0;
        ImageRef totalsize = from.size();

		while (!is_aligned<16>(rgb) || !is_aligned<16>(gray)) 
		{
			Pixel::CIE<Rgb<byte>,byte>::convert(*rgb, *gray);
			rgb++;
			gray++;
			count++;
		}
        
		if (count < totalsize.area())
			Internal::cvd_asm_rgb_to_gray((const byte*)rgb, gray, (totalsize.area()-count)&3, 77, 150, 29);

		count = (count + totalsize.area()) & 3;

		for(;count < totalsize.area(); count ++)
			Pixel::CIE<Rgb<byte>,byte>::convert(from.data()[count], to.data()[count]);
    }
}
