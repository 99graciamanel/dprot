import argparse
import os

import math
import numpy as np
from random import randint

NAMES = ["We", "I", "They", "He", "She", "Jack", "Jim"]
VERBS = ["was", "is", "are", "were"]
NOUNS = ["playing a game", "watching television", "talking", "dancing", "speaking"]

def parse_arguments():
    parser = argparse.ArgumentParser(formatter_class=argparse.ArgumentDefaultsHelpFormatter)
    parser.add_argument('--generate_random_docs', type=int, help="Number of nodes to be generated randomly", default=16)
    parser.add_argument('--proof_node', type=int, help="Node to be proved", default=16)
    return parser.parse_args()

def clean_directories():
    # Restart experiment folders
    os.system("rm -f nodes/*")
    os.system("rm -f proofs/*")

def generate_random_docs(number_of_docs):
    print("\nStart generating random docs----\n")
    for i in range(number_of_docs):
        generate_random_doc(i)
    print("\n----Finished generating random docs\n")

def generate_random_doc(doc_number):
    random_sentence = "%s %s %s" % (NAMES[randint(0, len(NAMES)-1)], VERBS[randint(0, len(VERBS)-1)], NOUNS[randint(0, len(NOUNS)-1)])
    new_doc = open("docs/doc%s.dat" % (doc_number), "w")
    new_doc.write("%s" % (random_sentence))
    new_doc.close()
    print("docs/doc%s.dat has been created randomly: %s" % (doc_number, random_sentence))

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
        self.n_docs = 0
        self.n_layers = 0
        self.node_matrix = np.zeros((2 ** 7, 2 ** 7))

    def build_merkle_tree(self, n_docs):
        self.n_docs = n_docs
        self.n_layers = self.calculate_n_layers(self.n_docs)

        print("Started Building Merkle Tree----\n")

        # Generate initial nodes from random docs
        for j in range(self.n_docs):
            self.node_matrix[0][j] = 1
            self.generate_doc_node(j, "nodes/node0.%s" % (j))
            print("0:%s:%s" % (j, read_bytes_from_file("nodes/node0.%s" % (j))))
        print()
        t = self.calculate_next_layer(self.n_docs)

        for i in range(1, self.n_layers):
            for j in range(t):
                self.node_matrix[i][j] = 1
                if self.node_matrix[i-1][2*j+1]: # If node is not alone
                    self.generate_node_from_two_nodes(
                        "nodes/node%s.%s" % (i-1, 2*j), "nodes/node%s.%s" % (i-1, 2*j+1), "nodes/node%s.%s" % (i,j))
                else:                            # If node is alone
                    self.generate_node_from_one_node(
                        "nodes/node%s.%s" % (i-1, 2*j), "nodes/node%s.%s" % (i,j))
                print("%s:%s:%s" % (i, j, read_bytes_from_file("nodes/node%s.%s" % (i, j))))
            print()
            t = self.calculate_next_layer(t)

        print("----Finished Building Merkle Tree\n")

    def formalize_results_document(self):
        print("Started writing summary to results file----\n")
        results = open("results", "w")

        results.write("MerkleTree:sha1:")

        doc_pre = read_bytes_from_file("prepends/doc.pre")
        node_pre = read_bytes_from_file("prepends/node.pre")

        root_node = read_bytes_from_file("nodes/node%s.0" % (self.n_layers-1))
        results.write("%s:%s:%s:%s:%s\n" % (doc_pre, node_pre, self.n_docs, self.n_layers, root_node))

        t = self.calculate_next_layer(self.n_docs)

        for i in range(self.n_layers):
            for j in range(t):
                results.write("%s:%s:%s\n" % (i, j, read_bytes_from_file("nodes/node%s.%s" % (i, j))))
            t = self.calculate_next_layer(t)

        results.close()
        print("----Finished writing summary to results file\n")

    def add_new_doc_to_tree(self):
        print("Started adding new doc to Merkle Tree----\n")
        generate_random_doc(self.n_docs)
        self.node_matrix[0][self.n_docs] = 1
        self.generate_doc_node(self.n_docs, "nodes/node0.%s" % (self.n_docs))
        print("0:%s:%s" % (self.n_docs, read_bytes_from_file("nodes/node0.%s" % (self.n_docs))))

        self.n_docs += 1
        self.n_layers = self.calculate_n_layers(self.n_docs)

        self.recalculate_new_doc_nodes()
        print("\n----Finished adding new doc to Merkle Tree\n")

    def recalculate_new_doc_nodes(self):
        t = self.calculate_next_layer(self.n_docs-1)
        for i in range(1, self.n_layers):
            self.node_matrix[i][t] = 1
            if self.node_matrix[i-1][2*t+1]: # If node is not alone
                self.generate_node_from_two_nodes(
                    "nodes/node%s.%s" % (i-1, 2*t), "nodes/node%s.%s" % (i-1, 2*t+1), "nodes/node%s.%s" % (i, t))
            else:                            # If node is alone
                self.generate_node_from_one_node(
                    "nodes/node%s.%s" % (i-1, 2*t), "nodes/node%s.%s" % (i, t))
            print("%s:%s:%s" % (i, t, read_bytes_from_file("nodes/node%s.%s" % (i, t))))
            t = self.calculate_next_layer(t)

    def produce_proof(self, id):
        proof = open("proofs/proof", "w")

        if id >= self.n_docs:
            proof.close()
            return False

        t = id
        # For each layer, decide which node is necessary for the proof (if an adjacent node exists)
        for i in range(self.n_layers-1):
            if t%2 == 0:
                if self.node_matrix[i][t + 1]: # If adjacent node exists
                    proof.write("nodes/node%s.%s" % (i, t+1))
            else:
                if self.node_matrix[i][t - 1]: # If adjacent node exists
                    proof.write("nodes/node%s.%s" % (i, t-1))
            proof.write("\n")

            t = self.calculate_next_layer(t)

        # Expected final proof
        proof.write("nodes/node%s.0" % (self.n_layers - 1))
        proof.close()
        return True

    def check_proof(self, id):
        print("Started checking proof----\n")
        if self.produce_proof(id) == False:
            print("################### Document not found in Merkle Tree ###################")
            print("\n----Finished checking proof")
            return

        t = id
        i = 0

        self.generate_doc_node(id, "proofs/proof0")
        proof = open("proofs/proof", "r")
        lines = proof.read().splitlines()
        print("%s:%s:%s" % (i, t, read_bytes_from_file("proofs/proof%s" % (i))))

        for i in range(self.n_layers - 1):
            proof_line = lines[i]
            # Depending on if it is pair or not, the order changes (proof may return an empty file)
            if t%2 == 0:
                self.generate_node_from_two_nodes(
                    "proofs/proof%s" % (i), proof_line, "proofs/proof%s" % (i+1))
            else:
                self.generate_node_from_two_nodes(
                    proof_line, "proofs/proof%s" % (i), "proofs/proof%s" % (i+1))

            t = self.calculate_next_layer(t)
            print("%s:%s:%s" % (i+1, t, read_bytes_from_file("proofs/proof%s" % (i+1))))

        proof.close()

        node_root = read_bytes_from_file(lines[self.n_layers - 1])
        print("\n\tExpected: \t%s" % (node_root))
        result = read_bytes_from_file("proofs/proof%s" % (i+1))
        print("\tResult: \t%s" % (result))

        if result == node_root:
            print("\nProof Check CORRECTLY")
        else:
            print("\nProof Check INCORRECTLY")
        print("\n----Finished checking proof")

    def calculate_n_layers(self, n_docs):
        return int(math.ceil(math.log2(n_docs))) + 1

    def calculate_next_layer(self, t):
        return int(np.floor(t/2))

    def generate_doc_node(self, doc_id, dest_file):
        os.system("cat prepends/doc.pre docs/doc%s.dat | openssl dgst -sha1 -binary > %s" % (doc_id, dest_file))

    def generate_node_from_two_nodes(self, first_node, second_node, dest_node):
        os.system("cat prepends/node.pre %s %s | openssl dgst -sha1 -binary > %s" % (first_node, second_node, dest_node))

    def generate_node_from_one_node(self, node, dest_node):
        os.system("cat prepends/node.pre %s | openssl dgst -sha1 -binary > %s" % (node, dest_node))

if __name__ == "__main__":
    args = parse_arguments()

    num_of_docs = args.generate_random_docs
    node_to_be_proved = args.proof_node

    clean_directories()
    generate_random_docs(num_of_docs)
    tree = MerkleTree()

    tree.build_merkle_tree(num_of_docs)
    tree.formalize_results_document()

    tree.add_new_doc_to_tree()
    tree.formalize_results_document()
    tree.check_proof(node_to_be_proved)