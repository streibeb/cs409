//
//  TextureManager.cpp
//
//  This file is part of the ObjLibrary, by Richard Hamilton,
//    which is copyright Hamilton 2009-2014.
//
//  You may use these files for any purpose as long as you do
//    not explicitly claim them as your own work or object to
//    other people using them.  If you are in a position of
//    authority, you may forbid others ffrom using them in areas
//    that fall under your authority.  For example, a professor
//    could forbid students from using them for a class project,
//    or an employer could forbid employees using for a company
//    project.
//
//  If you are destributing the source files, you must not
//    remove this notice.  If you are only destributing compiled
//    code, no credit is required.
//
//  A (theoretically) up-to-date version of the ObjLibrary can
//    be found at:
//  http://infiniplix.ca/resources/obj_library/
//

#include <cassert>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>

#include "GetGlut.h"
#include "ObjSettings.h"
#include "ObjStringParsing.h"
#include "Texture.h"
#include "TextureBmp.h"
#include "TextureManager.h"

#ifdef OBJ_LIBRARY_LOAD_PNG_TEXTURES
	#include "GetGlut.h"
	#include "../glpng.h"
#endif

using namespace std;
using namespace TextureManager;
namespace
{
	struct TextureData
	{
		string m_name;
		Texture m_texture;
	};

	// some sort of hash table would be nice
	//  -> convert to map<> when C++11 becomes more
	//     widely supported?
	vector<TextureData*> g_textures;

	Texture g_white;

	//
	//  getDummyTetxure
	//
	//  Purpose: To retrieve a reference to the dummy
	//	     texture.
	//  Parameter(s): N/A
	//  Precondition(s): N/A
	//  Returns: A reference to the dummy texture.
	//  Side Effect: If the dummy texture has not been
	//		 generated, it is generated.
	//

	const Texture& getDummyTexture ()
	{
		if(!g_white.isSet())
		{
			TextureBmp white;
			g_white.set(white.addToOpenGL());
		}

		return g_white;
	}
}



unsigned int TextureManager :: getCount ()
{
	return g_textures.size();
}

const Texture& TextureManager :: get (unsigned int index)
{
	assert(index < getCount());

	return g_textures[index]->m_texture;
}

const Texture& TextureManager :: get (const char* a_name)
{
	assert(a_name != NULL);

	return get(string(a_name), cerr);
}

const Texture& TextureManager :: get (const char* a_name, const string& logfile)
{
	assert(a_name != NULL);
	assert(logfile != "");

	ofstream logstream(logfile.c_str());
	const Texture& texture = get(string(a_name), logstream);
	logstream.close();

	return texture;
}

const Texture& TextureManager :: get (const char* a_name, ostream& r_logstream)
{
	assert(a_name != NULL);

	return get(string(a_name), r_logstream);
}

const Texture& TextureManager :: get (const string& name)
{
	return get(name, cerr);
}

const Texture& TextureManager :: get (const string& name, const string& logfile)
{
	assert(logfile != "");

	ofstream logstream(logfile.c_str());
	const Texture& texture = get(name, logstream);
	logstream.close();

	return texture;
}

const Texture& TextureManager :: get (const string& name, ostream& r_logstream)
{
	//  All gets and activates without wrapping and
	//    mag/minification parameters go through this line.
	return get(name, GL_REPEAT, GL_NEAREST, GL_NEAREST_MIPMAP_NEAREST, r_logstream);
}

const Texture& TextureManager :: get (const char* a_name,
                                      unsigned int wrap,
                                      unsigned int mag_filter,
                                      unsigned int min_filter)
{
	assert(a_name != NULL);
	assert(wrap == GL_REPEAT || wrap == GL_CLAMP);
	assert(mag_filter == GL_NEAREST ||
	       mag_filter == GL_LINEAR);
	assert(min_filter == GL_NEAREST ||
	       min_filter == GL_LINEAR ||
	       min_filter == GL_NEAREST_MIPMAP_NEAREST ||
	       min_filter == GL_NEAREST_MIPMAP_LINEAR ||
	       min_filter == GL_LINEAR_MIPMAP_NEAREST ||
	       min_filter == GL_LINEAR_MIPMAP_LINEAR);

	return get(string(a_name), wrap, mag_filter, min_filter, cerr);
}

const Texture& TextureManager :: get (const char* a_name,
                                      unsigned int wrap,
                                      unsigned int mag_filter,
                                      unsigned int min_filter,
                                      const std::string& logfile)
{
	assert(a_name != NULL);
	assert(wrap == GL_REPEAT || wrap == GL_CLAMP);
	assert(mag_filter == GL_NEAREST ||
	       mag_filter == GL_LINEAR);
	assert(min_filter == GL_NEAREST ||
	       min_filter == GL_LINEAR ||
	       min_filter == GL_NEAREST_MIPMAP_NEAREST ||
	       min_filter == GL_NEAREST_MIPMAP_LINEAR ||
	       min_filter == GL_LINEAR_MIPMAP_NEAREST ||
	       min_filter == GL_LINEAR_MIPMAP_LINEAR);
	assert(logfile != "");

	ofstream logstream(logfile.c_str());
	const Texture& texture = get(string(a_name), wrap, mag_filter, min_filter, logstream);
	logstream.close();

	return texture;
}

const Texture& TextureManager :: get (const char* a_name,
                                      unsigned int wrap,
                                      unsigned int mag_filter,
                                      unsigned int min_filter,
                                      std::ostream& r_logstream)
{
	assert(a_name != NULL);
	assert(wrap == GL_REPEAT || wrap == GL_CLAMP);
	assert(mag_filter == GL_NEAREST ||
	       mag_filter == GL_LINEAR);
	assert(min_filter == GL_NEAREST ||
	       min_filter == GL_LINEAR ||
	       min_filter == GL_NEAREST_MIPMAP_NEAREST ||
	       min_filter == GL_NEAREST_MIPMAP_LINEAR ||
	       min_filter == GL_LINEAR_MIPMAP_NEAREST ||
	       min_filter == GL_LINEAR_MIPMAP_LINEAR);

	return get(string(a_name), wrap, mag_filter, min_filter, r_logstream);
}

const Texture& TextureManager :: get (const std::string& name,
                                      unsigned int wrap,
                                      unsigned int mag_filter,
                                      unsigned int min_filter)
{
	assert(wrap == GL_REPEAT || wrap == GL_CLAMP);
	assert(mag_filter == GL_NEAREST ||
	       mag_filter == GL_LINEAR);
	assert(min_filter == GL_NEAREST ||
	       min_filter == GL_LINEAR ||
	       min_filter == GL_NEAREST_MIPMAP_NEAREST ||
	       min_filter == GL_NEAREST_MIPMAP_LINEAR ||
	       min_filter == GL_LINEAR_MIPMAP_NEAREST ||
	       min_filter == GL_LINEAR_MIPMAP_LINEAR);

	return get(name, wrap, mag_filter, min_filter, cerr);
}

const Texture& TextureManager :: get (const std::string& name,
                                      unsigned int wrap,
                                      unsigned int mag_filter,
                                      unsigned int min_filter,
                                      const std::string& logfile)
{
	assert(wrap == GL_REPEAT || wrap == GL_CLAMP);
	assert(mag_filter == GL_NEAREST ||
	       mag_filter == GL_LINEAR);
	assert(min_filter == GL_NEAREST ||
	       min_filter == GL_LINEAR ||
	       min_filter == GL_NEAREST_MIPMAP_NEAREST ||
	       min_filter == GL_NEAREST_MIPMAP_LINEAR ||
	       min_filter == GL_LINEAR_MIPMAP_NEAREST ||
	       min_filter == GL_LINEAR_MIPMAP_LINEAR);
	assert(logfile != "");

	ofstream logstream(logfile.c_str());
	const Texture& texture = get(name, wrap, mag_filter, min_filter, logstream);
	logstream.close();

	return texture;
}

const Texture& TextureManager :: get (const std::string& name,
                                      unsigned int wrap,
                                      unsigned int mag_filter,
                                      unsigned int min_filter,
                                      std::ostream& r_logstream)
{
	assert(wrap == GL_REPEAT || wrap == GL_CLAMP);
	assert(mag_filter == GL_NEAREST ||
	       mag_filter == GL_LINEAR);
	assert(min_filter == GL_NEAREST ||
	       min_filter == GL_LINEAR ||
	       min_filter == GL_NEAREST_MIPMAP_NEAREST ||
	       min_filter == GL_NEAREST_MIPMAP_LINEAR ||
	       min_filter == GL_LINEAR_MIPMAP_NEAREST ||
	       min_filter == GL_LINEAR_MIPMAP_LINEAR);

	string lower = toLowercase(name);
	unsigned int texture_count = g_textures.size();

	for(unsigned int i = 0; i < texture_count; i++)
		if(g_textures[i]->m_name == lower)
			return g_textures[i]->m_texture;

	if(endsWith(lower, ".bmp"))
	{
		TextureBmp texture_bmp(name.c_str(), r_logstream);
		if(texture_bmp.isBad())
		{
			// TextureBmp prints loading error
			return getDummyTexture();
		}
		else
		{
			//
			//  Texture is flipped when loading
			//    Should it be unflipped here?
			//    Standard is not clear
			//    Not flipping here matches Maya
			//
			//texture_bmp.mirrorY();
			//

			return add(texture_bmp.addToOpenGL(wrap, wrap, mag_filter, min_filter), lower);
		}
	}
	else if(endsWith(lower, ".png"))
	{
#ifdef OBJ_LIBRARY_LOAD_PNG_TEXTURES
		unsigned int texture_name = pngBind(name.c_str(),
		                                    PNG_BUILDMIPMAPS,
		                                    PNG_ALPHA,
		                                    NULL,
		                                    wrap,
		                                    min_filter,
		                                    mag_filter);
		if(texture_name == 0)
		{
			r_logstream << "Error: Loading failed: " << name << endl;
			return getDummyTexture();
		}
		else
			return add(texture_name, lower);
#else
		r_logstream << "Error: Loading .png textures is disabled: " << name << endl;
		return getDummyTexture();
#endif
	}
	else
	{
		r_logstream << "Error: Invalid image file extension: " << name << endl;
		return getDummyTexture();
	}
}

void TextureManager :: activate (unsigned int index)
{
	assert(index < getCount());

	g_textures[index]->m_texture.activate();
}

void TextureManager :: activate (const char* a_name)
{
	assert(a_name != NULL);

	get(string(a_name), cerr).activate();
}

void TextureManager :: activate (const char* a_name, const std::string& logfile)
{
	assert(a_name != NULL);
	assert(logfile != "");

	ofstream logstream(logfile.c_str());
	get(string(a_name), logstream).activate();
	logstream.close();
}

void TextureManager :: activate (const char* a_name, std::ostream& r_logstream)
{
	assert(a_name != NULL);

	get(string(a_name), r_logstream).activate();
}
		
void TextureManager :: activate (const std::string& name)
{
	get(name, cerr).activate();
}

void TextureManager :: activate (const std::string& name, const std::string& logfile)
{
	assert(logfile != "");

	ofstream logstream(logfile.c_str());
	get(name, logstream).activate();
	logstream.close();
}

void TextureManager :: activate (const std::string& name, std::ostream& r_logstream)
{
	get(name, r_logstream).activate();
}

void TextureManager :: activate (const char* a_name,
                                 unsigned int wrap,
                                 unsigned int mag_filter,
                                 unsigned int min_filter)
{
	assert(a_name != NULL);
	assert(wrap == GL_REPEAT || wrap == GL_CLAMP);
	assert(mag_filter == GL_NEAREST ||
	       mag_filter == GL_LINEAR);
	assert(min_filter == GL_NEAREST ||
	       min_filter == GL_LINEAR ||
	       min_filter == GL_NEAREST_MIPMAP_NEAREST ||
	       min_filter == GL_NEAREST_MIPMAP_LINEAR ||
	       min_filter == GL_LINEAR_MIPMAP_NEAREST ||
	       min_filter == GL_LINEAR_MIPMAP_LINEAR);

	get(string(a_name), wrap, mag_filter, min_filter, cerr).activate();
}

void TextureManager :: activate (const char* a_name,
                                 unsigned int wrap,
                                 unsigned int mag_filter,
                                 unsigned int min_filter,
                                 const std::string& logfile)
{
	assert(a_name != NULL);
	assert(wrap == GL_REPEAT || wrap == GL_CLAMP);
	assert(mag_filter == GL_NEAREST ||
	       mag_filter == GL_LINEAR);
	assert(min_filter == GL_NEAREST ||
	       min_filter == GL_LINEAR ||
	       min_filter == GL_NEAREST_MIPMAP_NEAREST ||
	       min_filter == GL_NEAREST_MIPMAP_LINEAR ||
	       min_filter == GL_LINEAR_MIPMAP_NEAREST ||
	       min_filter == GL_LINEAR_MIPMAP_LINEAR);
	assert(logfile != "");

	ofstream logstream(logfile.c_str());
	get(string(a_name), wrap, mag_filter, min_filter, logstream).activate();
	logstream.close();
}

void TextureManager :: activate (const char* a_name,
                                 unsigned int wrap,
                                 unsigned int mag_filter,
                                 unsigned int min_filter,
                                 std::ostream& r_logstream)
{
	assert(a_name != NULL);
	assert(wrap == GL_REPEAT || wrap == GL_CLAMP);
	assert(mag_filter == GL_NEAREST ||
	       mag_filter == GL_LINEAR);
	assert(min_filter == GL_NEAREST ||
	       min_filter == GL_LINEAR ||
	       min_filter == GL_NEAREST_MIPMAP_NEAREST ||
	       min_filter == GL_NEAREST_MIPMAP_LINEAR ||
	       min_filter == GL_LINEAR_MIPMAP_NEAREST ||
	       min_filter == GL_LINEAR_MIPMAP_LINEAR);

	get(string(a_name), wrap, mag_filter, min_filter, r_logstream).activate();
}

void TextureManager :: activate (const std::string& name,
                                 unsigned int wrap,
                                 unsigned int mag_filter,
                                 unsigned int min_filter)
{
	assert(wrap == GL_REPEAT || wrap == GL_CLAMP);
	assert(mag_filter == GL_NEAREST ||
	       mag_filter == GL_LINEAR);
	assert(min_filter == GL_NEAREST ||
	       min_filter == GL_LINEAR ||
	       min_filter == GL_NEAREST_MIPMAP_NEAREST ||
	       min_filter == GL_NEAREST_MIPMAP_LINEAR ||
	       min_filter == GL_LINEAR_MIPMAP_NEAREST ||
	       min_filter == GL_LINEAR_MIPMAP_LINEAR);

	get(name, wrap, mag_filter, min_filter, cerr).activate();
}

void TextureManager :: activate (const std::string& name,
                                 unsigned int wrap,
                                 unsigned int mag_filter,
                                 unsigned int min_filter,
                                 const std::string& logfile)
{
	assert(wrap == GL_REPEAT || wrap == GL_CLAMP);
	assert(mag_filter == GL_NEAREST ||
	       mag_filter == GL_LINEAR);
	assert(min_filter == GL_NEAREST ||
	       min_filter == GL_LINEAR ||
	       min_filter == GL_NEAREST_MIPMAP_NEAREST ||
	       min_filter == GL_NEAREST_MIPMAP_LINEAR ||
	       min_filter == GL_LINEAR_MIPMAP_NEAREST ||
	       min_filter == GL_LINEAR_MIPMAP_LINEAR);
	assert(logfile != "");

	ofstream logstream(logfile.c_str());
	get(name, wrap, mag_filter, min_filter, logstream).activate();
	logstream.close();
}

void TextureManager :: activate (const std::string& name,
                                 unsigned int wrap,
                                 unsigned int mag_filter,
                                 unsigned int min_filter,
                                 std::ostream& r_logstream)
{
	assert(wrap == GL_REPEAT || wrap == GL_CLAMP);
	assert(mag_filter == GL_NEAREST ||
	       mag_filter == GL_LINEAR);
	assert(min_filter == GL_NEAREST ||
	       min_filter == GL_LINEAR ||
	       min_filter == GL_NEAREST_MIPMAP_NEAREST ||
	       min_filter == GL_NEAREST_MIPMAP_LINEAR ||
	       min_filter == GL_LINEAR_MIPMAP_NEAREST ||
	       min_filter == GL_LINEAR_MIPMAP_LINEAR);

	get(name, wrap, mag_filter, min_filter, r_logstream).activate();
}



bool TextureManager :: isLoaded (const char* a_name)
{
	assert(a_name != NULL);

	return isLoaded(string(a_name));
}

bool TextureManager :: isLoaded (const string& name)
{
	string lower = toLowercase(name);

	for(unsigned int i = 0; i < g_textures.size(); i++)
		if(g_textures[i]->m_name == lower)
			return true;
	return false;
}

bool TextureManager :: isDummyTexture (const Texture& texture)
{
	if(texture == getDummyTexture())
		return true;
	else
		return false;
}



const Texture& TextureManager :: add (unsigned int opengl_name, const char* a_name)
{
	assert(opengl_name != 0);
	assert(a_name != NULL);
	assert(!isLoaded(a_name));

	return add(Texture(opengl_name), string(a_name));
}

const Texture& TextureManager :: add (unsigned int opengl_name, const string& name)
{
	assert(opengl_name != 0);
	assert(!isLoaded(name));

	return add(Texture(opengl_name), name);
}

const Texture& TextureManager :: add (const Texture& texture, const char* a_name)
{
	assert(texture.isSet());
	assert(a_name != NULL);
	assert(!isLoaded(a_name));

	return add(texture, string(a_name));
}

const Texture& TextureManager :: add (const Texture& texture, const string& name)
{
	assert(texture.isSet());
	assert(!isLoaded(name));

	string lower = toLowercase(name);
	unsigned int texture_count = g_textures.size();

	g_textures.push_back(new TextureData);
	g_textures[texture_count]->m_name = lower;
	g_textures[texture_count]->m_texture = texture;

	return g_textures[texture_count]->m_texture;
}

void unloadAll ()
{
	for(unsigned int i = 0; i < g_textures.size(); i++)
		delete g_textures[i];	// destructor frees video memory
	g_textures.clear();
}
