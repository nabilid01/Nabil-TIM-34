from flask import Flask, request, Response, jsonify
import json

app = Flask(__name__)

temp_list = []

@app.route('/')
def hello():
    return 'WELCOME TO MY JAPAN'

@app.route('/sensor/Japan', methods=["POST", "GET"])
def sensor():
    global temp_list  # Gunakan global untuk mengakses variabel temp_list di luar fungsi

    if request.method == 'POST':
        data = request.get_json()
        if data:
            temperature = data.get("Ondo")
            kelembapan = data.get("Shitsudo")
            timestamp = data.get("Jikanjunkan")

            if temperature is not None and kelembapan is not None and timestamp is not None:
                temp_list.append({
                    'Ondo': temperature,
                    'Shitsudo': kelembapan,
                    'Jikanjunkan': timestamp
                })

                response_data = {
                    'message': 'Data saved successfully'
                }
                status_code = 200
            else:
                response_data = {
                    'error': 'Invalid data format. Ensure temperature, humidity, and timestamp are provided.'
                }
                status_code = 400
        else:
            response_data = {
                'error': 'No JSON data received.'
            }
            status_code = 400

    elif request.method == 'GET':
        response_data = {
            'temperature_list': temp_list
        }
        status_code = 200

    response = jsonify(response_data)
    response.status_code = status_code
    return response

if __name__ == '__main__':
    app.run(debug=True, host='0.0.0.0')
