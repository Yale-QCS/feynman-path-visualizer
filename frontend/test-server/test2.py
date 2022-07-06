import json
import requests

url = 'http://44.195.46.250:23333'

payload = {
    'qubits': 3,
    'with_amp': False,
    'cols': [
        ['H'],
        ['.', 'X'],
        ['I', 'T'],
        ['I', 'T'],
        ['I', 'S'],
        ['I', '.', 'X'],
        ['H'],
        ['I', 'H'],
        ['X', '.'],
        ['Y', '.'],
        ['Y', '.'],
        ['I', 'T', '.'],
        ['I', 'T', '.'],
        ['I', 'S', '.'],
        ['I', 'Z', '.'],
        ['.', 'I', 'H'],
        ['.', 'I', 'H'],
        ['I', 'H']
    ]
}

malformatted_payload = {
    'qubits': 3,
    'with_amp': True,
    'cols': [
        ['R']
    ]
}

# Send HTTP post request and get the response.
mal_response = requests.post(url, json=malformatted_payload)

# Check the "success field".
mal_response_dict = json.loads(mal_response.text)
print('With malformatted input, "success" =', mal_response_dict['success'])

# Send HTTP post request and get the response.
response = requests.post(url, json=payload)

# Parse the response to a Python dict.
response_dict = json.loads(response.text)

# Make sure the invocation is successful.
assert response_dict['success']

print('Number of qubits:', response_dict['qubits'])
cnt = 0
for amplitude in response_dict['amplitudes']:

    # Print the amplitudes after each stage.
    print('Stage', cnt)
    for key, val in amplitude.items():

        # Check now done at the backend.
        # Skip very tiny amplitude caused by floating point error.
        # if val[0] ** 2 + val[1] ** 2 < 1e-6: continue

        # Print base and amplitude in the format "base  [real, imaginary]".
        print('%s\t [%.4f, %.4f]' % (key, val[0], val[1]))
    print()
    cnt += 1
