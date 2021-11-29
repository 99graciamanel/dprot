import argparse
import os
import math
import numpy as np

def parse_arguments():
    parser = argparse.ArgumentParser(formatter_class=argparse.ArgumentDefaultsHelpFormatter)
    parser.add_argument('--generate_random_docs', type=int, help="Number of nodes to be generated randomly", default=8)
    parser.add_argument('--proof_node', type=int, help="Node to be proved", default=0)
    return parser.parse_args()

def clean_directories():
    # Remove past nodes
    os.system("rm -rf nodes")
    os.system("rm -rf proofs")

    os.system("mkdir nodes/")
    os.system("mkdir proofs/")

def generate_random_docs(number_of_docs):
    for i in range(number_of_docs):
        generate_random_doc(i)

def generate_random_doc(doc_number):
    new_doc = open("docs/doc" + str(doc_number) + ".dat", "w")
    new_doc.write("This is the " + str(doc_number) + " random sentence")
    new_doc.close()

def read_bytes_from_file(file_name):
    file = open(file_name, "rb")
    value = ""
    byte = file.read(1)
    while byte != b"":
        value += format(ord(byte), "02x")
        byte = file.read(1)
    file.close()
    return value

class MerkleTree:
    def __init__(self):
        self.n = 0
        self.layers = 0
        self.nodes = np.zeros((0,0))

    def build_tree(self, n):
        # Create a leaf for each document
        # i = 0
        self.n = n
        self.layers = int(math.ceil(math.log2(n))) + 1
        self.nodes = np.zeros((2**7,2**7)) # not n --> need to check if node exists or not

        for j in range(self.n):
            self.nodes[0][j] = 1
            os.system("cat prepends/doc.pre docs/doc"+str(j)+".dat | openssl dgst -sha1 -binary > nodes/node0."+str(j))

        t = int(np.floor(self.n / 2))

        for i in range(1, self.layers):
            for j in range(t):
                self.nodes[i][j] = 1
                if self.nodes[i-1][2*j+1]:
                    # leaf exists
                    os.system("cat prepends/node.pre nodes/node" + str(i-1) + "." + str(2*j) + " nodes/node" + str(i-1) + "." + str(2*j+1) + " | openssl dgst -sha1 -binary > nodes/node" + str(i) + "."+str(j))
                else:
                    # leaf does not exist
                    os.system("cat prepends/node.pre nodes/node" + str(i-1) + "." + str(2*j) + " | openssl dgst -sha1 -binary > nodes/node" + str(i) + "."+str(j))
                print(str(i) + ":" + str(j) + ":" + read_bytes_from_file("nodes/node" + str(i)+ "."+str(j)))
            t = int(np.floor(t / 2))

    def write_summary(self):
        results = open("results", "w")

        results.write("MerkleTree:sha1:")

        doc_pre = read_bytes_from_file("prepends/doc.pre")
        node_pre = read_bytes_from_file("prepends/node.pre")

        results.write(doc_pre + ":" + node_pre)

        results.write(":" + str(self.n) + ":" + str(self.layers) + ":")

        root_node = read_bytes_from_file("nodes/node"+str(self.layers-1)+".0")

        results.write(root_node)
        results.write("\n")

        t = int(np.floor(self.n / 2))

        for i in range(self.layers):
            for j in range(t):
                node = read_bytes_from_file("nodes/node" + str(i) + "." + str(j))
                results.write(str(i) + ":" + str(j) + ":" + node + "\n")
            t = int(np.floor(t / 2))
        results.close()

    def add_new_doc(self):
        new_doc = open("docs/doc"+str(self.n)+".dat", "w")
        new_doc.write("This is the " + str(self.n) + " random sentence")
        new_doc.close()
        self.nodes[0][self.n] = 1
        os.system("cat prepends/doc.pre docs/doc"+str(self.n)+".dat | openssl dgst -sha1 -binary > nodes/node0."+str(self.n))
        self.n += 1
        self.layers = int(math.ceil(math.log2(self.n))) + 1

    def add_file(self):
        j = int(np.floor((self.n-1)/2))
        for i in range(1, self.layers):
            self.nodes[i][j] = 1
            if self.nodes[i-1][2*j+1]:
                # leaf exists
                os.system("cat prepends/node.pre nodes/node" + str(i-1) + "." + str(2*j) + " nodes/node" + str(i-1) + "." + str(2*j+1) + " | openssl dgst -sha1 -binary > nodes/node" + str(i) + "."+str(j))
            else:
                # leaf does not exist
                os.system("cat prepends/node.pre nodes/node" + str(i-1) + "." + str(2*j) + " | openssl dgst -sha1 -binary > nodes/node" + str(i) + "."+str(j))
            j = int(np.floor(j / 2))

    def produce_proof(self, id):
        proof = open("proofs/proof", "w")

        if id >= self.n:
            proof.write("Document does not belong to Merkle Tree.\n")
            proof.close()
            return False

        j = id
        for i in range(self.layers-1):
            node = ""
            if j%2 == 0:
                if self.nodes[i][j+1]:
                    proof.write("nodes/node"+str(i)+"."+str(j+1))
                proof.write("\n")
            else:
                if self.nodes[i][j-1]: # don't think this is necessary
                    proof.write("nodes/node"+str(i)+"."+str(j-1))
                proof.write("\n")

            j = int(np.floor(j/2))

        proof.write("nodes/node"+str(self.layers-1)+".0")
        proof.close()
        return True

    def check_proof(self, id):
        if self.produce_proof(id) == False:
            return

        j = id
        i = 0

        os.system("cat prepends/doc.pre docs/doc"+str(id)+".dat | openssl dgst -sha1 -binary > proofs/proof0")

        proof = open("proofs/proof", "r")
        lines = proof.read().splitlines()
        for i in range(self.layers-1):
            proof_line = lines[i]

            if j%2 == 0:
                os.system("cat prepends/node.pre proofs/proof" + str(i) + " " + proof_line + " | openssl dgst -sha1 -binary > proofs/proof" + str(i+1))
            else:
                os.system("cat prepends/node.pre " + proof_line + " proofs/proof" + str(i) + " | openssl dgst -sha1 -binary > proofs/proof" + str(i+1))
            print(str(i) + ":" + str(j) + ":" + read_bytes_from_file("proofs/proof" + str(i)))
            j = int(np.floor(j/2))

        node_root = read_bytes_from_file(lines[self.layers-1])
        proof.close()
        result = read_bytes_from_file("proofs/proof" + str(i+1))

        if result == node_root:
            print("CORRECT!")
        else:
            print("INCORRECT!")

        return

if __name__ == "__main__":
    args = parse_arguments()

    num_of_docs = args.generate_random_docs
    node_to_be_proved = args.proof_node

    clean_directories()
    generate_random_docs(num_of_docs)
    tree = MerkleTree()

    tree.build_tree(num_of_docs)
    tree.write_summary()
    tree.add_new_doc()
    tree.add_file()
    tree.write_summary()
    tree.check_proof(node_to_be_proved)