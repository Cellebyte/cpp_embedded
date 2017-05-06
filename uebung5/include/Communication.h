/*
 *  @author Marcel Fest
 */
class ClientServer
{
    public:
        enum Mode : unsigned int
        {
            Server,
            Client
        };

        bool Start ( Mode mode , unsigned int port , const char * ip );
};
