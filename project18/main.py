from requests_html import HTMLSession
import  requests

session = HTMLSession()
url = 'https://www.blockchain.com/explorer/transactions/btc/94a93ba7d3a8729edc4a48139edf117e4abb0929c746daae7c08e8e16b66f7ff'
response = session.get(url)
print(response.html.full_text)

url='https://en.bitcoin.it/wiki/Testnet'
tx="202100460142"
print("发送的tx信息为：",tx)
post_html=requests.post(url,data=tx)
print("响应的信息为：")
print(post_html.text)