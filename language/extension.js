// The module 'vscode' contains the VS Code extensibility API
// Import the module and reference it with the alias vscode in your code below
const vscode = require('vscode');

let underline = vscode.window.createTextEditorDecorationType(
	{ borderStyle: "solid", borderColor: new vscode.ThemeColor('textLink.activeForeground'), borderWidth: "0 0 2px 0" });

let highlight = vscode.window.createTextEditorDecorationType(
	// { borderStyle: "solid", borderColor: "#FF0000", borderWidth: "0 0 2px 0" });
	{ backgroundColor: new vscode.ThemeColor('editor.findMatchHighlightBackground') });

/**
 * @param {vscode.ExtensionContext} context
 */
function activate(context) {

	// Use the console to output diagnostic information (console.log) and errors (console.error)
	// This line of code will only be executed once when your extension is activated
	console.log('Forgscript Language Extension Active');

	// Set the background text color using the provided input
	function updateDecorations(underlined, highlighted, editor) {
		// Apply this highlight color (style) to the specified line (ranges)
		editor.setDecorations(underline, underlined);
		editor.setDecorations(highlight, highlighted);
	}

	function collatz(x) {
		return x % 2 == 0 ? collatz_even(x) : collatz_odd(x)
	}
	function collatz_even(x) {
		return x / 2;
	}
	function collatz_odd(x) {
		return 3 * x + 1
	}

	function getDecoratorRanges(char, x, y) {

		function getRange(x, y) {
			let pos = new vscode.Position(y - 1, x - 1);
			return new vscode.Range(pos, pos.translate(0, 1))
		}

		if (char == 'v') {
			return [getRange(collatz(x), y + 1)]
		} else if (char == '^') {
			return [getRange(collatz(x), y - 1)]
		} else if (char == '*') {
			return x % 2 == 0 ? [getRange(collatz_even(x), y), getRange(collatz_odd(x), y)] : [getRange(collatz(x), y)]
		} else {
			return [getRange(collatz(x), y)]
		}
	}

	function getNextSymbol(pos, editor) {
		function getRange(pos) {
			return new vscode.Range(pos, pos.translate(0, 1))
		}
		let newPos
		if (getCharAt(pos, editor).match(/[v]/)) {
			newPos = pos.translate(1);
		} else if (getCharAt(pos, editor).match(/\^/)){
			newPos = pos.translate(-1);
		} else {
			newPos = pos.with();
		}
		function isNoop(char) {
			return !char.match(/[v^<>\+\-\*]/)
		}
		for (let i = 0; i == 0 || i < 200 && isNoop(getCharAt(newPos, editor)); i++) {
			newPos = newPos.with(undefined, collatz(newPos._character + 1) - 1)
		}
		if (!isNoop(getCharAt(newPos, editor))) {
			return getRange(newPos);
		}
		return null;
	}

	function getCharAt(pos, editor) {
		return editor.document.getText(new vscode.Range(pos, pos.translate(0, 1)))
	}

	function isUnitSelection(selections) {
		if (selections.length != 1) return false;
		let selection = selections[0];
		if (selection.isEmpty) return true;
		if (selection._end._line - selection._start._line == 0 && selection._end._character - selection._start._character <= 1) return true;
		return false;
	}

	// Event Handler: This event fires when the selected text changes
	vscode.window.onDidChangeTextEditorSelection(e => {
		if (e.textEditor.document.languageId == "forgscript" && isUnitSelection(e.selections)) {
			let pos = e.selections[0]._start
			let char = getCharAt(pos, e.textEditor)
			let stepOptions = getDecoratorRanges(char, pos._character + 1, pos._line + 1);
			let action = getNextSymbol(pos, e.textEditor);
			let nextAction = action ? [action] : []
			updateDecorations(
				vscode.workspace.getConfiguration('forgscript').get('underlineNextStep') ? stepOptions : [],
				vscode.workspace.getConfiguration('forgscript').get('highlightNextAction') ? nextAction : [], e.textEditor);
		} else {
			updateDecorations([], [], e.textEditor);
		}
	}, null, context.subscriptions);

}



// this method is called when the extension is deactivated
function deactivate() {
	// Remove the text highlighting when the plugin is terminated
	if (style !== undefined) {
		style.dispose();
	}
}

module.exports = {
	activate,
	deactivate
}
