/***************************************************************************
 *   Copyright (C) 2007 by Mark Glenn                                      *
 *   markglenn@gmail.com                                                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU Lesser General Public License as        *
 *   published by the Free Software Foundation; either version 2.1 of the  *
 *   License, or (at your option) any later version.                       *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU Lesser General Public License for more details.                   *
 *                                                                         *
 *   You should have received a copy of the GNU Lesser General Public      *
 *   License along with this program; if not, write to the                 *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#ifndef _MINIUI_UISYSTEM_WIDGET_H_
#define _MINIUI_UISYSTEM_WIDGET_H_

#include "../Graphics/Renderable.h"
#include <string>
#include <luabind/luabind.hpp>
#include <MiniUI/TinyXPath/tinyxml.h>
#include <MiniUI/LuaSystem/LuaVirtualMachine.h>
#include "../Input/Mouse.h"

namespace MiniUI
{
	namespace Widgets
	{
		class Widget;

		typedef std::vector< Widget* > WidgetList;
		struct WidgetChildArea
		{
			Types::Rectangle<int> 	area;
			WidgetList 				children;
			int 					depth;
		};

		typedef std::vector<WidgetChildArea> WidgetChildren;

		class Widget
		{
		public:

			Widget ( );
			virtual ~Widget ( );

			static void RegisterWithLua ( LuaSystem::LuaVirtualMachine* );

			void SetRenderable ( Graphics::Renderable* pRenderable );

			void AddWidgetChildArea ( WidgetChildArea childArea )
				{ _widgetChildren.push_back ( childArea ); }
			WidgetChildren* GetWidgetChildren ( ) { return &_widgetChildren; }

			Graphics::Renderable* GetRenderable ( ) { return _pRenderable; }

			void SetName ( std::string name ) { _name = name; }
			std::string Name ( ) { return _name; }

			void HandleMouseInput ( Input::Mouse *pMouse );

			// Lua methods //
			virtual void Update ( int timestep ) { };

			Graphics::GraphicalRect* GetGraphicalRect ( std::string id );
			void UpdateRenderable ( ) { _pRenderable->OnChanged(); }

			void SetAngle ( float angle );
			float GetAngle ( ) const;

			void SetPositionX ( int x ) { _pRenderable->position.x = x; }
			void SetPositionY ( int y ) { _pRenderable->position.y = y; }
			int GetPositionX ( ) const { return _pRenderable->position.x; }
			int GetPositionY ( ) const { return _pRenderable->position.y; }

			float GetOpacity ( ) const { return _pRenderable->opacity; }
			void SetOpacity ( float o ) { _pRenderable->opacity = o; }

			void SetWidth ( int width ) { _pRenderable->size.x = width; }
			int GetWidth ( ) const { return _pRenderable->size.x; }
			void SetHeight ( int height ) { _pRenderable->size.y = height; }
			int GetHeight ( ) const { return _pRenderable->size.y; }

			virtual void OnLoad ( TinyXPath::TiXmlElement *pSkin, TinyXPath::TiXmlElement *pLayout ) {}
			virtual void OnLayout ( ) { }

			virtual void Call ( std::string func, luabind::object object ) {}
			void CallVoid ( std::string func ) { Call ( func, luabind::object () ); }
			
			int GetAreaCount ( ) { return _widgetChildren.size(); }
			int GetChildWidgetCount ( int area );
			
			Widget* GetChildWidget ( int area, int widget );
			Widget* GetWidgetByID ( std::string id );
			
			void AddChild ( int area, Widget *pWidget );
			
			void Destroy ( );

			Types::Vector2D<int>* GetPosition ( ) const;
			
			// widget.id
			void SetID ( std::string id ) { _id = id; }
			std::string GetID ( ) const { return _id; }
			
			void Fire ( std::string event, luabind::object const& object );

			virtual void OnMouseOver ( ) 					{ }
			virtual void OnMouseOut ( ) 					{ }
			virtual void OnMouseHover ( int x, int y )		{ }
			virtual void OnMouseDown ( int x, int y )		{ }
			virtual void OnMouseMove ( int x, int y )		{ }
			virtual void OnMouseUp ( ) 						{ }
			
		private:
			Graphics::Renderable 	*_pRenderable;
			WidgetChildren			_widgetChildren;
			
			std::string				_name;
			std::string				_id;
			
			bool _isMouseOver;
			bool _isMouseDown;
			bool _ignoreState;
		};

		class Widget_wrapper : public Widget, public luabind::wrap_base
		{
		public:
			Widget_wrapper()
				: Widget()
			{ }

			virtual void Update( int timestep )
			{
				call<void>("Update", timestep);
			}

			static void default_Update ( Widget* ptr, int timestep )
			{
				return ptr->Widget::Update( timestep );
			}

			virtual void OnLoad ( TinyXPath::TiXmlElement *pSkin, TinyXPath::TiXmlElement *pLayout )
			{
				call<void>("OnLoad", pSkin, pLayout);
			}

			static void default_OnLoad ( Widget* ptr, TinyXPath::TiXmlElement *pSkin, TinyXPath::TiXmlElement *pLayout )
			{
				return ptr->Widget::OnLoad ( pSkin, pLayout );
			}

			virtual void OnLayout ( )
			{
				call<void>( "OnLayout" );
			}

			static void default_OnLayout ( Widget* ptr )
			{
				return ptr->Widget::OnLayout ( );
			}
			
			virtual void Call ( std::string func, luabind::object object )
			{
				call<void>( "Call", func, object );
			}

			static void default_Call ( Widget* ptr, std::string func, luabind::object object )
			{
				ptr->Widget::Call ( func, object );
			}

			
			virtual void OnMouseOver ( ) 	{ call<void> ("OnMouseOver" ); }
			virtual void OnMouseOut ( )		{ call<void> ("OnMouseOut" ); }
			virtual void OnMouseUp ( )		{ call<void> ("OnMouseUp" ); }
			
			virtual void OnMouseHover ( int x, int y )
			{
				call<void>( "OnMouseHover", x, y );
			}
			
			virtual void OnMouseMove ( int x, int y )
			{
				call<void>( "OnMouseMove", x, y );
			}
			
			virtual void OnMouseDown ( int x, int y )
			{
				call<void>( "OnMouseDown", x, y );
			}
			
			static void default_OnMouseOver ( Widget* ptr )
			{
				ptr->Widget::OnMouseOver ( );
			}
			
			static void default_OnMouseOut ( Widget* ptr )
			{
				ptr->Widget::OnMouseOut ( );	
			}
			
			static void default_OnMouseUp ( Widget* ptr )
			{
				ptr->Widget::OnMouseUp ( );
			}
			
			static void default_OnMouseHover ( Widget* ptr, int x, int y )
			{
				ptr->Widget::OnMouseHover ( x, y );
			}
			
			static void default_OnMouseMove ( Widget* ptr, int x, int y )
			{
				ptr->Widget::OnMouseMove ( x, y );
			}
						
			static void default_OnMouseDown ( Widget* ptr, int x, int y )
			{
				ptr->Widget::OnMouseDown ( x, y );
			}

		};
	}
}

#endif // _MINIUI_UISYSTEM_WIDGET_H_
