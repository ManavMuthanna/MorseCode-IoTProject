from flask import Flask, render_template, request, jsonify, session, Response
from flask_caching import Cache

app = Flask(__name__)
app.secret_key = 'secret'

# Configure Flask cache
cache = Cache(app, config={'CACHE_TYPE': 'simple'})

morse_code_mappings = {
    '._': 'A', '_...': 'B', '_._.': 'C', '_..': 'D', '.': 'E',
    '.._.': 'F', '__.': 'G', '....': 'H', '..': 'I', '.___': 'J',
    '_._': 'K', '._..': 'L', '__': 'M', '_.': 'N', '___': 'O',
    '.__.': 'P', '__._': 'Q', '._.': 'R', '...': 'S', '_': 'T',
    '.._': 'U', '..._': 'V', '.__': 'W', '_.._': 'X', '_.__': 'Y',
    '__..': 'Z', '.____': '1', '..___': '2', '...__': '3', '...._': '4',
    '.....': '5', '_....': '6', '__...': '7', '___..': '8', '____.': '9',
    '_____': '0', '__..__': ',', '._._._': '.', '..__..': '?', '_.._.': '/',
    '_...._': '_', '_.__.': '(', '_.__._': ')', '._...': '&', '___...': ':',
    '_._._.': ';', '_..._': '=', '._._.': '+', '_...._': '_', '..__._': '_',
    '._.._.': '"', '..._.._': '$', '.__._.': '@', '...___...': 'SOS'
}

code = "Waiting"
english = "Waiting"

@app.route("/")
def home():
    return render_template("index.html")

@app.route("/data", methods=["POST"])
def push_dots_and_dashes():
    if request.method == "POST":
        output = request.get_json()
        result = output.get("text", "")
        
        # Translate Morse code to English
        translated_english = translate_morse_to_english(result)

        # Update the global English translation
        global code, english
        code = result
        english = translated_english

        print("Received dots and dashes: " + result)
        print("Translated to English: " + translated_english)

        return jsonify(success=True)

@app.route("/stream")
def stream():
    return jsonify({"code": code, "english" : english})

def translate_morse_to_english(morse_code):
    if morse_code is None:
        return ""
    
    words = morse_code.split(' / ')
    translated_words = []

    for word in words:
        letters = word.split(' ')
        translated_letters = []

        for letter in letters:
            if letter in morse_code_mappings:
                translated_letters.append(morse_code_mappings[letter])

        translated_words.append(''.join(translated_letters))

    return ' '.join(translated_words)


if __name__ == "__main__":
    app.run(host='0.0.0.0', debug=True)
