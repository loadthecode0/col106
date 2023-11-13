import requests
import sys
import json


# class MakeApiCall:

def get_data(api):
    response = requests.get(f"{api}")
    if response.status_code == 200:
        print("sucessfully fetched the data")
        formatted_print(response.json())
    else:
        print(
            f"Hello person, there's a {response.status_code} error with your request")

def get_user_data(api, parameters):
    response = requests.post(f"{api}", data=parameters)
    if response.status_code == 200:
        print("sucessfully fetched the data with parameters provided")
        print("self.formatted_print(): ")
        formatted_print(response.json())
        response_json = json.loads(response.text)["text"]
        print("response.json(): ", response.json())
        # response__ = json.loads(response_json)
        # print(response__)
        resp_str = json.dumps(response_json)
        print("aList: ", resp_str)
        return resp_str
    else:
        print(
            f"Hello person, there's a {response.status_code} error with your request")

def formatted_print(obj):
    text = json.dumps(obj, sort_keys=True, indent=4)
    print(text)

# def __init__(self, api, text):
#     # self.get_data(api)

#     parameters = {
#         "text": text
#     }
    
#     return self.get_user_data(api, parameters)


if __name__ == '__main__':
    
    # python3 <filename> API_KEY num_paragraphs query.txt
    # if len(sys.argv) < 4:
    #     print("Usage: python3 api_call.py API_KEY num_paragraphs query.txt")
    #     sys.exit(1)
    print("Hello")

    # Read the API call from the command line
    # openai.api_key = sys.argv[1]
    # num_paragraphs = int(sys.argv[2])
    # print(num_paragraphs)

    # # Read the raw query tokens from the command line
    print (len(sys.argv))
    print (sys.argv)
    parsedCmd = sys.argv[1:]
    
    print(parsedCmd)
    
    bagCap = 2*len(parsedCmd) #4*no.of raw tokens
    print(bagCap)
    
    #calculate total of frequencies for proportion calc
    totalFreq = sum(int(parsedCmd[i]) for i in range(1, len(parsedCmd), 2))
    print(totalFreq)
    
    relWtDict = {}
    
    filename = 'query.txt' #contains processed tokens
    print(filename)
    with open(filename, 'a') as f:
        for i in range(0, len(parsedCmd), 2):
            rawToken = parsedCmd[i]
            paramDict = {"text": rawToken}
            api_call_response = get_user_data("http://text-processing.com/api/tag/", paramDict)
            print(api_call_response)
            rawFreq = parsedCmd[i+1]
            print(rawToken, rawFreq)
            relWtDict[rawToken] = (int(rawFreq)*bagCap/totalFreq) #round off

    print(relWtDict)
    
    # # add query
    # query_file = sys.argv[3]
    # with open(query_file, 'r') as f:
    #     query = f.read()
    #     paragraphs.append(query)
    #     paragraphs.append('\n')

    # # convert paragraphs to a single string
    # paragraphs = '\n'.join(paragraphs)

    # print(paragraphs)

    # query = {
    #     "role": "user", "content": paragraphs
    # }

    # chat = openai.ChatCompletion.create(
    #     model="gpt-3.5-turbo",
    #     messages=[query]
    # )

    # reply = chat.choices[0].message.content
    # print(reply)