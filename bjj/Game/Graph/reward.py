import json
import os


def load_json(fpath):
    with open(fpath, 'r') as file:
        return json.load(file)


def add_terminal_win_states(G, json_path=None):
    """add annotations of which nodes are considered a win """
    if json_path is None:
        json_path = os.path.join('Graph', 'files', 'terminal_node_winstate.json')
    with open(json_path, 'r') as file:
        terminal_win_nodes = json.load(file)
    for node_dict in terminal_win_nodes:
        G.nodes[node_dict['node']]['winner'] = node_dict['winner']
    return G
def add_tap_flag(G):
    """Marks that this move is a tap, telling game engine that this player lost"""
    # to do: 4 of the 'tap' transitions have properties that don't match the position

    for node in G.nodes():
        # Get all outgoing edges
        out_edges = list(G.out_edges(node, data=True))
        # Check if there's only one outgoing edge and if its description is "tap"
        if len(out_edges) == 1 and out_edges[0][2].get('description') == 'tap':
            # Get the edge data
            source, target, edge_data = out_edges[0]
            # Add a new attribute to the edge
            G.edges[source, target]['tap'] = True

    return G

def flag_point_earning_move(G,move: str):
    """
    Checks the 'tags' values of every transition in the graph for the inputted flag, then adds a new edge attribute with a boolean.
    This is meant to be used to check for transitions that should earn points for the player who executed it
    """
    def is_move_in_tags(start,end, move=move, G=G) -> bool:
        # checks if flag is in any of the edge's tags
        tags = G.edges[start,end].get('tags')
        if tags:
            return any([move in tag for tag in tags])
        return False

    for u, v in G.edges():
        if is_move_in_tags(u,v):
            G.edges[u, v][move] = True
    return G


def find_move_by_node_tags(G, position: str):
    """
    finds transitions from:
    node without <position> in tags --> node with <position> in tags

    and then marks that transition with a new dict item
    """
    # to do: 3 of the mount transitions need to have their top/bottom tags switched
    def node_is_position(node: str,position=position ,G=G) -> bool:
        #checks if position is in any of the node's tags
        tags = G.nodes[node].get('tags')
        if tags:
            return any([position in tag for tag in tags])
        return False

    for node in G.nodes():
        # if this node does not have <position> in tags
        if not node_is_position(node):
            # Check all successors of the current node
            for successor in G.successors(node):
                # Check if the successor node has <position>
                if node_is_position(successor):
                    # Transition to the node with the position is found, adding flag to this edge
                    G.edges[node, successor][position] = True

    return G

def find_and_tag_all_moves(G):
    # sweeps
    G = flag_point_earning_move(G, 'sweep')
    # mounts
    G = find_move_by_node_tags(G, 'mount')
    # backtakes
    G = find_move_by_node_tags(G, 'back')
    # takedowns: note that both of these describe the same point earning move
    G = flag_point_earning_move(G, 'throw')
    G = flag_point_earning_move(G, 'takedown')
    # guard passes
    G = flag_point_earning_move(G, 'pass')
    # knee on belly
    # to do
    return G

def add_rewards_to_graph(G):
    ## Identifying terminal game states
    # Flagging positions where one player has won. This identified checkmates positions to terminate the game at
    G = add_terminal_win_states(G)
    # Identifying moves where one player submits and flagging it
    G = add_tap_flag(G)

    ## Identifying point-earning moves and tagging them
    G = find_and_tag_all_moves(G)
    return G



