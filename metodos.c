Object Document{
	FiguresVector();
	TempVector();
}

figure(){
	ListPoints();
	fillingColor();
	Grosor();
}

//Dibujar Panel Herramientas
DrawPanel(){
	documentActions(){
		NewDocument(){}
		SaveDocument(){}
		CloseDocument(){}
	}
	
	clipBoardActions(){
		CopyElement(){}
		CutElement(){}
		PasteElement(){}
	}
	
	RecoveryActions(){
		undoElement(){}
		redoElement(){}
	}
	
	ToolsActions(){
		DrawForm(){
			Polygon(){}
			Line(){}
			circle(){}
		}
		colors(){}
		pencil(){}
		fill(){}
		erase(){}
		text(){}
	}
	
}

//Panel de Selección de vectores 
DrawVector(){
	listForms(){}
	
}

//Panel Dibujado
DrawWorkSpace(){
	DrawVector(){}
	GetMouse(){}
	
}