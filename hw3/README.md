# Develop Hyperldger Fabric Applications

## How to run

Set up a development environment:

First deploy a network with 2 peers and 1 ordering service. One script that takes care of the network staff can be found at https://github.com/hyperledger/fabric-samples/tree/master/test-network.
```
./network.sh up createChannel -c mychannel -ca
```

Then deploy the codechain implemented in `chaincode-go/` to the network. Name the contract as `mycontract`.
```
./network.sh deployCC -ccn mycontract -ccp /chaincode-go/ -ccl go
```

In order to run the application, just go to the `application-go/` directory, and
```
go run .
```

----
## Concepts
### [The Ledger](https://hyperledger-fabric.readthedocs.io/en/latest/developapps/analysis.html)
The Ledger
In this topic, we’ve seen how transactions and the resultant paper states are the two most important concepts in PaperNet. Indeed, we’ll see these two fundamental elements in any Hyperledger Fabric distributed ledger:
- a world **state**, that contains the current value of all objects, and 
- a **blockchain** that records the history of all transactions that resulted in the current world state.

The required sign-offs on transactions are enforced through rules, which are evaluated before appending a transaction to the ledger. Only if the required signatures are present, Fabric will accept a transaction as valid.

### Ledger State
A Fabric state is implemented as a key/value pair, in which the value encodes the object properties in a format that captures the object’s multiple properties, typically JSON.

### Smart Contract
A smart contract defines 
- the different **states** of a business object, and 
- the **transactions** that move the object between these different states. 

Smart contracts are important because they allow architects and smart contract developers to define the key business processes and data that are shared across the different organizations collaborating in a blockchain network.

### Application
An application can interact with a blockchain network by submitting transactions to a ledger or querying ledger content.

An application has to follow six basic steps to submit a transaction:

- Select an identity from a wallet
    - a wallet holds a set of identities – X.509 digital certificates – which can be used to access Fabric network.
- Connect to a gateway
    - a gateway identifies one or more peers that provide access to a network
- Access the desired network
    - the gateway actually provides the application with access to multiple network channels
- Construct a transaction request for a smart contract
    - get contract from the chaincode file that may implement many contracts
- Submit the transaction to the network
    - a single method call to the SDK, will invoke the transaction function implemented in smart contract
- Process the response

### Chaincode
A chaincode is a generic container for deploying code to a Hyperledger Fabric blockchain network. 
One or more related smart contracts are defined within a chaincode.

Once a chaincode has been installed on a peer and deployed to a channel, the smart contracts in it are accessible to an application:

