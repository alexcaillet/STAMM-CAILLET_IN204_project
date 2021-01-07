#include <iostream>
#include "tinyxml2.h"



int main(int argc, char const *argv[])
{
    using namespace tinyxml2;
    XMLDocument doc;
    doc.LoadFile( "test.xml" );
    
    XMLElement *l_pRootElement = doc.RootElement();
 
    if( NULL != l_pRootElement )
    {
    // set of &lt;person&gt; tags
        XMLElement *l_pPeople = l_pRootElement->FirstChildElement( "people" );
 
        if ( NULL != l_pPeople )
        {
            XMLElement *l_pPerson = l_pPeople->FirstChildElement( "person" );
     
            while( l_pPerson )
            {
                XMLElement *l_pForename = l_pPerson->FirstChildElement( "forename" );
     
                if ( NULL != l_pForename )
                {
                    std::cout << l_pForename->GetText();
                }
     
                XMLElement *l_pSurname = l_pPerson->FirstChildElement( "surname" );
     
                if ( NULL != l_pSurname )
                {
                    std::cout << " " << l_pSurname->GetText();
                }
     
                std::cout << std::endl;
     
                l_pPerson = l_pPerson->NextSiblingElement( "person" );
            }
        }
    }
}
