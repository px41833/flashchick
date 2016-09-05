/*
   Test program for TinyXML.
*/


#ifdef TIXML_USE_STL
	#include <iostream>
	#include <sstream>
	using namespace std;
#else
	#include <stdio.h>
#endif

#if defined( WIN32 ) && defined( TUNE )
	#include <crtdbg.h>
	_CrtMemState startMemState;
	_CrtMemState endMemState;
#endif

#include "tinyxml.h"

bool XmlTest (const char* testString, const char* expected, const char* found, bool noEcho = false);
bool XmlTest( const char* testString, int expected, int found, bool noEcho = false );

static int gPass = 0;
static int gFail = 0;



bool XmlTest (const char* testString, const char* expected, const char* found, bool noEcho )
{
	bool pass = !strcmp( expected, found );
	if ( pass )
		printf ("[pass]");
	else
		printf ("[fail]");

	if ( noEcho )
		printf (" %s\n", testString);
	else
		printf (" %s [%s][%s]\n", testString, expected, found);

	if ( pass )
		++gPass;
	else
		++gFail;
	return pass;
}


bool XmlTest( const char* testString, int expected, int found, bool noEcho )
{
	bool pass = ( expected == found );
	if ( pass )
		printf ("[pass]");
	else
		printf ("[fail]");

	if ( noEcho )
		printf (" %s\n", testString);
	else
		printf (" %s [%d][%d]\n", testString, expected, found);

	if ( pass )
		++gPass;
	else
		++gFail;
	return pass;
}


void NullLineEndings( char* p )
{
	while( p && *p ) {
		if ( *p == '\n' || *p == '\r' ) {
			*p = 0;
			return;
		}
		++p;
	}
}

//
// This file demonstrates some basic functionality of TinyXml.
// Note that the example is very contrived. It presumes you know
// what is in the XML file. But it does test the basic operations,
// and show how to add and remove nodes.
//

int main()
{

	//
	// We start with the 'demoStart' todo list. Process it. And
	// should hopefully end up with the todo list as illustrated.
	//
	const char* demoStart =
		"<?xml version=\"1.0\"  standalone='no' >\n"
		"<!-- Our to do list data -->"
		"<ToDo>\n"
		"<!-- Do I need a secure PDA? -->\n"
		"<Item priority=\"1\" distance='close'> Go to the <bold>Toy store!</bold></Item>"
		"<Item priority=\"2\" distance='none'> Do bills   </Item>"
		"<Item priority=\"2\" distance='far &amp; back'> Look for Evil Dinosaurs! </Item>"
		"</ToDo>";
		
	{



		{
			// Write to a file and read it back, to check file I/O.

			TiXmlDocument doc( "pxtest.xml" );
			doc.Parse( demoStart );

			if ( doc.Error() )
			{
				printf( "Error in %s: %s\n", doc.Value(), doc.ErrorDesc() );
				exit( 1 );
			}
			doc.SaveFile();
		}

		TiXmlDocument doc( "demotest.xml" );
		bool loadOkay = doc.LoadFile();

		if ( !loadOkay )
		{
			printf( "Could not load test file 'demotest.xml'. Error='%s'. Exiting.\n", doc.ErrorDesc() );
			exit( 1 );
		}

		printf( "** Demo doc read from disk: ** \n\n" );
		printf( "** Printing via doc.Print **\n" );
		doc.Print( stdout );

		{
			printf( "** Printing via TiXmlPrinter **\n" );
			TiXmlPrinter printer;
			doc.Accept( &printer );
			fprintf( stdout, "%s", printer.CStr() );
		}


		
	
	{

		const char* passages =
			"<?xml version=\"1.0\" standalone=\"no\" ?>"
			"<passages count=\"006\" formatversion=\"20020620\">"
				"<psg context=\"Line 5 has &quot;quotation marks&quot; and &apos;apostrophe marks&apos;."
				" It also has &lt;, &gt;, and &amp;, as well as a fake copyright &#xA9;.\"> </psg>"
			"</passages>";

		TiXmlDocument doc( "passages.xml" );
		doc.Parse( passages );
		TiXmlElement* psg = doc.RootElement()->FirstChildElement();
		const char* context = psg->Attribute( "context" );
		const char* expected = "Line 5 has \"quotation marks\" and 'apostrophe marks'. It also has <, >, and &, as well as a fake copyright \xC2\xA9.";

		XmlTest( "Entity transformation: read. ", expected, context, true );

		FILE* textfile = fopen( "textfile.txt", "w" );
		if ( textfile )
		{
			psg->Print( textfile, 0 );
			fclose( textfile );
		}
		textfile = fopen( "textfile.txt", "r" );
		assert( textfile );
		if ( textfile )
		{
			char buf[ 1024 ];
			fgets( buf, 1024, textfile );
			XmlTest( "Entity transformation: write. ",
					 "<psg context=\'Line 5 has &quot;quotation marks&quot; and &apos;apostrophe marks&apos;."
					 " It also has &lt;, &gt;, and &amp;, as well as a fake copyright \xC2\xA9.' />",
					 buf,
					 true );
		}
		fclose( textfile );
	}

    {
		FILE* textfile = fopen( "test5.xml", "w" );
		if ( textfile )
		{
            fputs("<?xml version='1.0'?><a.elem xmi.version='2.0'/>", textfile);
            fclose(textfile);

			TiXmlDocument doc;
            doc.LoadFile( "test5.xml" );
            XmlTest( "dot in element attributes and names", doc.Error(), 0);
		}
    }

	{
		FILE* textfile = fopen( "test6.xml", "w" );
		if ( textfile )
		{
            fputs("<element><Name>1.1 Start easy ignore fin thickness&#xA;</Name></element>", textfile );
            fclose(textfile);

            TiXmlDocument doc;
            bool result = doc.LoadFile( "test6.xml" );
            XmlTest( "Entity with one digit.", result, true );

			TiXmlText* text = doc.FirstChildElement()->FirstChildElement()->FirstChild()->ToText();
			XmlTest( "Entity with one digit.",
						text->Value(), "1.1 Start easy ignore fin thickness\n" );
		}
    }

	{
		// DOCTYPE not preserved (950171)
		// 
		const char* doctype =
			"<?xml version=\"1.0\" ?>"
			"<!DOCTYPE PLAY SYSTEM 'play.dtd'>"
			"<!ELEMENT title (#PCDATA)>"
			"<!ELEMENT books (title,authors)>"
			"<element />";

		TiXmlDocument doc;
		doc.Parse( doctype );
		doc.SaveFile( "test7.xml" );
		doc.Clear();
		doc.LoadFile( "test7.xml" );
		
		TiXmlHandle docH( &doc );
		TiXmlUnknown* unknown = docH.Child( 1 ).Unknown();
		XmlTest( "Correct value of unknown.", "!DOCTYPE PLAY SYSTEM 'play.dtd'", unknown->Value() );

	}

	{

		const char* doctype = 
			"<!-- Somewhat<evil> -->";
		TiXmlDocument doc;
		doc.Parse( doctype );

		TiXmlHandle docH( &doc );
		TiXmlComment* comment = docH.Child( 0 ).Node()->ToComment();

		XmlTest( "Comment formatting.", " Somewhat<evil> ", comment->Value() );
		
	}

	{
		// [ 870502 ] White space issues
		TiXmlDocument doc;
		TiXmlText* text;
		TiXmlHandle docH( &doc );
	
		const char* doctype0 = "<element> This has leading and trailing space </element>";
		const char* doctype1 = "<element>This has  internal space</element>";
		const char* doctype2 = "<element> This has leading, trailing, and  internal space </element>";

		TiXmlBase::SetCondenseWhiteSpace( false );
		doc.Clear();
		doc.Parse( doctype0 );
		text = docH.FirstChildElement( "element" ).Child( 0 ).Text();
		XmlTest( "White space kept.", " This has leading and trailing space ", text->Value() );

		doc.Clear();
		doc.Parse( doctype1 );
		text = docH.FirstChildElement( "element" ).Child( 0 ).Text();
		XmlTest( "White space kept.", "This has  internal space", text->Value() );

		doc.Clear();
		doc.Parse( doctype2 );
		text = docH.FirstChildElement( "element" ).Child( 0 ).Text();
		XmlTest( "White space kept.", " This has leading, trailing, and  internal space ", text->Value() );

		TiXmlBase::SetCondenseWhiteSpace( true );
		doc.Clear();
		doc.Parse( doctype0 );
		text = docH.FirstChildElement( "element" ).Child( 0 ).Text();
		XmlTest( "White space condensed.", "This has leading and trailing space", text->Value() );

		doc.Clear();
		doc.Parse( doctype1 );
		text = docH.FirstChildElement( "element" ).Child( 0 ).Text();
		XmlTest( "White space condensed.", "This has internal space", text->Value() );

		doc.Clear();
		doc.Parse( doctype2 );
		text = docH.FirstChildElement( "element" ).Child( 0 ).Text();
		XmlTest( "White space condensed.", "This has leading, trailing, and internal space", text->Value() );
	}


	

	printf ("\nPass %d, Fail %d\n", gPass, gFail);
	return gFail;
}
}
