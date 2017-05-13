#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Sun May  7 14:31:43 2017

@author: shaffer
"""
import pandas as pd
import numpy as np
import networkx as nx
import matplotlib.pyplot as plt

def log_to_df_server(path):
    df = pd.read_csv(path,
                 sep=' ', header=None, error_bad_lines=False, warn_bad_lines=False, 
                 parse_dates=[1,2], infer_datetime_format=True, 
                 names=['IP', 'time_server', 'time_node', 'channel', 'rssi', 'nodeID', 'address', 'data'],
                 dtype={'IP'           : str,
                        'channel'      : float,
                        'rssi'         : str,
                        'nodeID'       : int, 
                        'address'      : int,
                        'data'         : str
                       })

    df['latency'] = (df.time_server - df.time_node)/np.timedelta64(1, 's')
    df = df[df.nodeID !=-1]
    return df

def log_to_df(path):
    df = pd.read_csv(path,
                 sep=' ', header=None, error_bad_lines=False, warn_bad_lines=False,
                 parse_dates=[0], infer_datetime_format=True, 
                 names=['time_node', 'channel', 'rssi', 'nodeID', 'address', 'data'],
                 dtype={'channel'      : int,
                        'rssi'         : str,
                        'nodeID'       : int,
                        'address'      : int,
                        'data'         : str
                       })

    df = df[df.nodeID !=-1]
    return df


def range_mask(column, start, end=''):
    if end == '':
        end = column[-1]
    mask = (column > start) & (column < end)
    return mask

def plot_network(net_config):
    net_config.reset_index(drop=True, inplace=True)
    G = nx.DiGraph()
    G.add_node(0)
    
    for row in net_config.index:
        address = str(net_config.address.values[row]) + '0'
        
        parent = address[1:]
        child = address[:-1]
        
        if parent != '0':
            parent = parent[:-1]
        
        G.add_edge(int(parent),int(child))
    
    labels = dict(zip(net_config.address, net_config.nodeID))
    labels[0] = 0
    
    pos = nx.spring_layout(G)
    nx.draw(G, pos, node_size=800, alpha=0.65, node_color='b')
    nx.draw_networkx_labels(G,pos,labels, font_size=14, font_weight='bold',
                                                        font_color='w')
    plt.show()
    return G
