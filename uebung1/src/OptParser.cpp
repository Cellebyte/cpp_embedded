class CmdLineOptParser
{
    public:
        bool Parse(int argc, char* argv[]);
    protected:
        virtual bool Option(const char c , const char* info);
};


bool CmdLineOptParser::Parse(int argc, char* argv[])
{
    for (int i=1; i<argc;i++)
    {
        char* argument=argv[i];
        if(argument[0]!='-') return false;
        if(argument[1]=='\0') return false;
        char option=argv[i][1];
        if(argument[2]!='\0')
        {
            int j=2;
            int begin;
            if(argument[2]=='=')
            {
                j++;
                begin=3;
            }
            else
            {
                begin=2;
            }
            while(argument[j]!='\0')
            {
                argument[j-begin]=argument[j];
                j++;
            }
            j++;
            argument[j]='\0';
            if(! Option(option,argument)) return false;
        }
        else
        {
            i++;
            if(! Option(option,argv[i])) return false;
        }
    }
    return true;
}
bool CmdLineOptParser::Option(const char c , const char* info)
{
    if(c!='\0' and info!='\0') return true;
	else return false;
}