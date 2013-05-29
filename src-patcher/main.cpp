#include <windows.h>
#include <stdio.h>
#include <string>
#include <sstream>
#include <algorithm>

#define EXE_NAME "ePSXe.exe"
#define DLL_NAME "shark.dll"

#define BASE 0x00400000

struct exe_info
{
	unsigned long exe_version;
	unsigned long exe_size;
	unsigned long oep_pos;
	unsigned long ep_pos;
	unsigned long loadlibrary;
	unsigned long code_pos;
};

exe_info exes[] = 
{
	{
		// ePSXe 1.8.0
		/*exe_version*/              180,
		/*exe_size*/             1523712,
		/*oep_pos*/           0x00049CBB,
		/*peep_pos*/               0x138,
		/*loadlibrary*/       0x00459160,
		/*code_pos*/             0x58800,
	},
	{
		// ePSXe 1.7.0
		/*exe_version*/              170,
		/*exe_size*/             1437696,
		/*oep_pos*/           0x0003F1A5,
		/*peep_pos*/               0x130,
		/*loadlibrary*/       0x0044D158,
		/*code_pos*/             0x4CF70,
	},
	{
		// ePSXe 1.6.0
		/*exe_version*/              160,
		/*exe_size*/              536576,
		/*oep_pos*/           0x0004046C,
		/*peep_pos*/               0x140,
		/*loadlibrary*/       0x00448098,
		/*code_pos*/             0x47F00,
	},
	{
		// ePSXe 1.5.2
		/*exe_version*/              152,
		/*exe_size*/              544768,
		/*oep_pos*/           0x0003EF55,
		/*peep_pos*/               0x140,
		/*loadlibrary*/       0x00447098,
		/*code_pos*/             0x46C00,
	}
};


#pragma pack(push, r1, 1)  
struct stub_struct
{
	unsigned char push;
	unsigned long name;
	unsigned char call[3];
	unsigned long loadlibrary;
	unsigned char moveax;
	unsigned long oep;
	unsigned char jmpeax[2];
};
#pragma pack(pop, r1)

stub_struct stub =
{
	/*push*/         0x68,
	/*name*/         0,
	/*call*/         {0x3E,0xFF,0x15},
	/*loadlibrary*/  0,
	/*moveax*/       0xB8,
	/*oep*/          0,
	/*jmpeax*/       {0xFF,0xE0}
};

void error(std::string error = std::string("Unknown error"))
{
	MessageBoxA(NULL,error.c_str(),"Error",MB_ICONERROR);
	exit(1);
}

int check(FILE* fp, exe_info *e)
{
	//placeholder for checks
	unsigned long test=0xFF;

	// size check
	size_t size=0;
	fseek(fp, 0L, SEEK_END);
	size = ftell(fp);
	if(size != e->exe_size) return 0;

	// check code
	unsigned char codecheck[sizeof(stub_struct)];
	fseek(fp,e->code_pos,SEEK_SET);
	fread(&codecheck,sizeof(stub_struct),1,fp);
	for(unsigned char c : codecheck) if(c) return 0;

	// check data
	unsigned long name_pos = e->code_pos + sizeof(stub_struct);
	fseek(fp,name_pos,SEEK_SET);
	fread(&test,sizeof(test),1,fp);
	if(test) return 0;

	// check EP
	fseek(fp,e->ep_pos,SEEK_SET);
	fread(&test,sizeof(test),1,fp);
	if(test != e->oep_pos) return 0;

	return 1;
}

void patch(FILE* fp, exe_info *e)
{
	size_t written=0;

	unsigned long name_pos = e->code_pos + sizeof(stub_struct);
	unsigned long oep = BASE + e->oep_pos;
	unsigned long name = BASE + name_pos;

	stub.name = name;
	stub.loadlibrary = e->loadlibrary;
	stub.oep = oep;

	// write code
	fseek(fp,e->code_pos,SEEK_SET);
	written = fwrite(&stub,sizeof(stub_struct),1,fp);
	if(!written) error("Code write failed");

	// write data
	std::string dll = DLL_NAME;
	fseek(fp,name_pos,SEEK_SET);
	written = fwrite(dll.c_str(),dll.length(),1,fp);
	if(!written) error("Data write failed");

	// write new EP
	unsigned long ep = e->code_pos;
	fseek(fp,e->ep_pos,SEEK_SET);
	written = fwrite(&ep,sizeof(ep),1,fp);
	if(!written) error("EntryPoint write failed");
}

std::string wtoa(const std::wstring& wide)
{
	std::string out;
	std::transform(wide.begin(), wide.end(), std::back_inserter(out),
		[](wchar_t ch) -> char { return static_cast<char>(ch); });
	return out;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
				   LPTSTR lpCmdLine, int nCmdShow)
{
	FILE* fp;
	errno_t err;

	wchar_t** argv;
	int argc;

	std::string exe_name;

	argv = CommandLineToArgvW(GetCommandLineW(),&argc);
	if(argc > 1)
	{
		for(int i = 1; i < argc; i++ )
		{
			wchar_t* arg = *(argv+i);
			exe_name = wtoa(arg);
		}
	}
	else exe_name = EXE_NAME;

	err = fopen_s(&fp,exe_name.c_str(),"r+b");
	if(err) error(std::string(exe_name) + " file not found");

	for(exe_info &e : exes) 
	{
		if(check(fp,&e))
		{
			patch(fp,&e);
			fclose(fp);

			int major = e.exe_version/100;
			int minor = (e.exe_version-100)/10;
			int build = (e.exe_version - major*100) - minor*10;

			std::stringstream ss;
			ss << "ePSXe " << major << "." << minor << "." << build << " patched successfully" << std::endl << "Now run " << exe_name << " as normal.";
			std::string &str = ss.str();

			MessageBoxA(NULL,str.c_str(),"Success",MB_ICONINFORMATION);
			return 0;
		}
	}

	if(fp) fclose(fp);
	error();
	return 1;
}
