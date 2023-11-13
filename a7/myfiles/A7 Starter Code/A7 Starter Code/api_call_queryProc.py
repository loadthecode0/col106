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

def get_pos(parameters):
    api = "http://text-processing.com/api/tag/"
    response = requests.post(f"{api}", data=parameters)
    if response.status_code == 200:
        print("sucessfully fetched the data with parameters provided")
        # formatted_print(response.json())
        response_json = json.loads(response.text)["text"]
        resp_str = json.dumps(response_json)
        print(resp_str)
        if resp_str[-6:-3] in ["NNP", "NNPS"]:
            return "propNoun"
        else:
            return "other"
    else:
        print(
            f"Hello person, there's a {response.status_code} error with your request")

def get_stem(parameters):
    api = "http://text-processing.com/api/stem/"
    response = requests.post(f"{api}", data=parameters)
    if response.status_code == 200:
        print("sucessfully fetched the data with parameters provided")
        # formatted_print(response.json())
        response_json = json.loads(response.text)["text"]
        resp_str = json.dumps(response_json)
        print(resp_str)
        return resp_str[1:-1]
    else:
        print(
            f"Hello person, there's a {response.status_code} error with your request")
        
def get_meaningful_stem(stem):
    print ("get meaningful stem of", stem)
    api = "https://api.datamuse.com/words?sl="
    api += stem
    response = requests.get(f"{api}&max=1")
    if response.status_code == 200:
        print("sucessfully fetched the data with parameters provided")
        formatted_print(response.json())
        response_json = json.loads(response.text)[0]["word"]
        resp_str = json.dumps(response_json)
        print(resp_str)
        return resp_str[1:-1]
    else:
        print(
            f"Hello person, there's a {response.status_code} error with your request")

def get_synonyms(stem, num):
    print ("get synonyms of", stem)
    api = "https://api.datamuse.com/words?rel_syn="
    api += stem
    response = requests.get(f"{api}&max={num}")
    if response.status_code == 200:
        print("sucessfully fetched the data with parameters provided")
        formatted_print(response.json())
        response_json = json.loads(response.text)[:num]
        myList = []
        for x in range(len(response_json)) :
            response_json_x = json.loads(response.text)[x]["word"]
            myList.append(response_json_x)
        print(myList)
        return myList
        # resp_str = json.dumps(response_json)
        # print(resp_str)
        # return resp_str
    else:
        print(
            f"Hello person, there's a {response.status_code} error with your request")

def formatted_print(obj):
    text = json.dumps(obj, sort_keys=True, indent=4)
    print(text)
    
    
def processToken (token, posTagNo, num):
    if (posTagNo == 0): #NNP or NNPS
        print("Hello proper noun")
        #write to file
    # else :
        
        
    
    


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
    weightDict = {}
    for i in range(0, len(parsedCmd), 2):
        rawToken = parsedCmd[i]
        rawFreq = parsedCmd[i+1]
    
    totalInvFreq = sum((1/int(parsedCmd[i])) for i in range(1, len(parsedCmd), 2))
    print(totalInvFreq)
    
    relWtDict = {}
    
    filename = 'query.txt' #contains processed tokens
    print(filename)
    with open(filename, 'w') as f:
        for i in range(0, len(parsedCmd), 2):
            rawToken = parsedCmd[i]
            paramDict = {"text": rawToken}
            pos = get_pos( paramDict)
            # print(api_call_response)
            if pos == "propNoun":
                f.write(f"{rawToken}!\n") #can modify marker for proper noun
            else:
                f.write(f"{rawToken}\n")
                rawInvFreq = parsedCmd[i+1]
                print(rawToken, rawInvFreq)
                relWtDict[rawToken] = round((1/int(rawInvFreq))*bagCap/totalInvFreq) #round off
                
                # api_call_response = get_user_data("http://text-processing.com/api/tag/", paramDict)
                stem = get_stem(paramDict)
                print(len(stem))
                meaningfulStem = get_meaningful_stem(stem)
                print(meaningfulStem)
                if (meaningfulStem != rawToken):
                    f.write(f"{meaningfulStem}\n")
                synList = get_synonyms(meaningfulStem, relWtDict[rawToken])
                for x in synList:
                    if (x != rawToken):
                        f.write(f"{x}\n")
                
                
                

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