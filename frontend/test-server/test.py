import json
import requests

url = 'https://lvx53rzzu7.execute-api.us-east-1.amazonaws.com/default/Quantum_Single_Full'

payload = {
    'qubits': 3,
    'cols': [
        ['H'],
        ['•', 'X'],
        [1, 'Z'],
        [1, '•', 'X'],
        ['H'],
        [1, 'H'],
        ['X', '•'],
        [1, 'H']
    ]
}

# Send HTTP post request and get the response.
response = requests.post(url, json=payload)

# Parse the response to a Python dict.
response_dict = json.loads(response.text)

print('Number of qubits:', response_dict['qubits'])
cnt = 0
for amplitude in response_dict['amplitudes']:

    # Print the amplitudes after each stage.
    print('Stage', cnt)
    for key, val in amplitude.items():

        # Skip very tiny amplitude caused by floating point error.
        if val[0] ** 2 + val[1] ** 2 < 1e-6: continue

        # Print base and amplitude in the format "base  [real, imaginary]".
        print('%s\t [%.4f, %.4f]' % (key, val[0], val[1]))
    print()
    cnt += 1
