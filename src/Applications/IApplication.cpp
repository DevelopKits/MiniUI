#include "IApplication.h"

using namespace MiniUI;
using namespace MiniUI::EventSystem;

namespace Applications
{
	///////////////////////////////////////////////////////////////////////////
	IApplication::IApplication ( MiniUI::UIManager *pManager )
	///////////////////////////////////////////////////////////////////////////
	{
		_pManager = pManager;
		
		_pEventFunctor = new Functor<IApplication> (this, &IApplication::OnUIEvent );
	}
	
	///////////////////////////////////////////////////////////////////////////
	IApplication::~IApplication ( )
	///////////////////////////////////////////////////////////////////////////
	{
		Hide ( );
		delete _pEventFunctor;
	}
	
	///////////////////////////////////////////////////////////////////////////
	void IApplication::OnUIEvent ( Event *pEvent )
	///////////////////////////////////////////////////////////////////////////
	{
		OnEvent ( static_cast<UIEvent *>(pEvent) );
	}
	
	///////////////////////////////////////////////////////////////////////////
	void IApplication::Show ( )
	///////////////////////////////////////////////////////////////////////////
	{
		_pManager->OnUIEvent.AddListener ( _pEventFunctor );
	}
	
	///////////////////////////////////////////////////////////////////////////
	void IApplication::Hide ( )
	///////////////////////////////////////////////////////////////////////////
	{
		_pManager->OnUIEvent.RemoveListener ( _pEventFunctor );
	}
}
