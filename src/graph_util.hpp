#ifndef GRAPPLEMAP_GRAPH_UTIL_HPP
#define GRAPPLEMAP_GRAPH_UTIL_HPP

#include "graph.hpp"
#include <map>
#include <boost/range/counting_range.hpp>
#include <boost/range/adaptor/transformed.hpp>

namespace GrappleMap {

inline NodeNum::range nodenums(Graph const & g) { return {NodeNum{0}, NodeNum{g.num_nodes()}}; }
inline SeqNum::range seqnums(Graph const & g) { return {SeqNum{0}, SeqNum{g.num_sequences()}}; }

SeqNum insert(Graph &, Sequence const &);
optional<SeqNum> erase_sequence(Graph &, SeqNum);
optional<SeqNum> split_at(Graph &, PositionInSequence);

// first/last/next/prev/end/from/to

inline PosNum end(Sequence const & s)
{ return {PosNum::underlying_type(s.positions.size())}; }

inline PosNum last_pos(Sequence const & s)
{
	return {PosNum::underlying_type(s.positions.size() - 1)};
}

inline Location pos_loc(PositionInSequence const pis, Graph const & g)
{
	Sequence const & seq = g[pis.sequence];

	if (pis.position == last_pos(seq))
		return Location
			{ SegmentInSequence
			  { pis.sequence
			  , SegmentNum{uint8_t(pis.position.index-1)} }, 1};
	else
		return Location
			{ SegmentInSequence
			  { pis.sequence
			  , SegmentNum{pis.position.index} }, 0 };
}

inline PositionInSequence last_pos_in(SeqNum const s, Graph const & g)
{
	return s * last_pos(g[s]);
}

template<typename T>
auto last_pos_in(Reoriented<T> const s, Graph const & g)
{
	return last_pos_in(*s, g) * s.reorientation;
}

template<typename T>
auto first_pos_in(Reversible<T> const s, Graph const & g)
{
	return s.reverse ? last_pos_in(*s, g) : first_pos_in(*s);
}

template<typename T>
auto first_pos_in(Reoriented<T> const s, Graph const & g)
{
	return first_pos_in(*s, g) * s.reorientation;
}

inline PosNum::range posnums(Graph const & g, SeqNum const s)
{
	return {PosNum{0}, end(g[s])};
}

inline auto positions(Graph const & g, SeqNum const s)
{
	return posnums(g, s) | transformed([s](PosNum p){ return s * p; });
}

inline auto positions(Graph const & g, Reoriented<SeqNum> const s)
	// returns a range of Reoriented<PositionInSequence>
{
	return positions(g, *s) | transformed(
		[s](PositionInSequence const pis) { return pis * s.reorientation; });
}

inline SegmentNum last_segment(Sequence const & s)
{
	return {SegmentNum::underlying_type(s.positions.size() - 2)};
		// e.g. 3 positions means 2 segments, so last segment index is 1
}

inline SegmentInSequence last_segment(SeqNum const s, Graph const & g)
{
	return s * last_segment(g[s]);
}

inline Reversible<SegmentInSequence> first_segment(Reversible<SeqNum> const & s, Graph const & g)
{
	return {s.reverse ? last_segment(*s, g) : first_segment(*s), s.reverse};
}

inline Location start_loc(Reversible<SeqNum> const & s, Graph const & g)
{
	return start_loc(first_segment(s, g));
}

inline Reversible<SegmentInSequence> last_segment(Reversible<SeqNum> const & s, Graph const & g)
{
	return {s.reverse ? first_segment(*s) : last_segment(*s, g), s.reverse};
}

template <typename T>
auto last_segment(Reoriented<T> const & s, Graph const & g)
{
	return last_segment(*s, g) * s.reorientation;
}

template <typename T>
auto first_segment(Reoriented<T> const & s, Graph const & g)
{
	return first_segment(*s, g) * s.reorientation;
}

template <typename T>
optional<Reoriented<T>> next(Reoriented<T> const & s, Graph const & g)
{
	if (auto x = next(*s, g)) return *x * s.reorientation;
	return boost::none;
}

inline optional<PositionInSequence> next(PositionInSequence const pis, Graph const & g)
{
	if (pis.position == last_pos(g[pis.sequence])) return none;
	return pis.sequence * next(pis.position);
}

inline auto const & to(SeqNum const s, Graph const & g)
{
	return g[s].to;
}

inline auto const & from(SeqNum const s, Graph const & g)
{
	return g[s].from;
}

inline Reoriented<NodeNum> from(Reoriented<SeqNum> const & s, Graph const & g)
{
	Reoriented<NodeNum> const & n = g[*s].from;
	return *n * compose(n.reorientation, s.reorientation);
}

inline Reoriented<NodeNum> to(Reoriented<SeqNum> const & s, Graph const & g)
{
	Reoriented<NodeNum> const & n = g[*s].to;
	return *n * compose(n.reorientation, s.reorientation);
}

inline Reoriented<NodeNum> from(Reoriented<Reversible<SeqNum>> const & s, Graph const & g)
{
	return s->reverse
		? to(forget_direction(s), g)
		: from(forget_direction(s), g);
}

inline Reoriented<NodeNum> to(Reoriented<Reversible<SeqNum>> const & s, Graph const & g)
{
	return from(reverse(s), g);
}

inline SegmentNum end_segmentnum(Sequence const & s)
{
	return {SegmentNum::underlying_type(s.positions.size() - 1)};
}

inline SegmentNum::underlying_type num_segments(Sequence const & s)
{
	return s.positions.size() - 1;
}

inline SegmentNum::range segments(Sequence const & s)
{
	return {SegmentNum{0}, end_segmentnum(s)};
}

inline PosNum end_pos(Sequence const & s)
{
	return PosNum{PosNum::underlying_type(s.positions.size())};
}

inline PosNum::range positions(Sequence const & s) { return {PosNum{0}, end_pos(s)}; }

inline auto positions(Reoriented<SeqNum> const & s, Graph const & g)
{
	return positions(g[*s]) | transformed([s](PosNum const p){ return s * p; });
}

inline auto segments(Reoriented<SeqNum> const & seq, Graph const & g)
{
	return segments(g[*seq]) | transformed([seq](SegmentNum seg){ return seq * seg; });
}

inline vector<Reoriented<Reversible<SegmentInSequence>>>
	segments(Reoriented<Step> const & step, Graph const & g)
{
	vector<Reoriented<Reversible<SegmentInSequence>>> v;

	foreach (seg : segments(g[**step]))
		v.push_back(Reoriented<Reversible<SegmentInSequence>>
			{ {{**step, seg}, step->reverse}
			, step.reorientation
			});
	
	if (step->reverse) std::reverse(v.begin(), v.end());

	return v;
}

// in/out

Reoriented<Reversible<SeqNum>>
	gp_connect(Reoriented<NodeNum> const &, Reversible<SeqNum>, Graph const &);

inline auto connector(Reoriented<NodeNum> const n, Graph const & g)
{
	return transformed([&g, n](Reversible<SeqNum> const s) { return gp_connect(n, s, g); });
}

inline auto in_sequences(Reoriented<NodeNum> const & n, Graph const & g)
{
	return g[*n].in | connector(n, g);
}

inline auto out_sequences(Reoriented<NodeNum> const & n, Graph const & g)
{
	return g[*n].out | connector(n, g);
}

inline auto in_segments(Reoriented<NodeNum> const & n, Graph const & g)
	// returns a range of Reoriented<Reversible<SegmentInSequence>>
{
	return in_sequences(n, g) | transformed(
		[&](Reoriented<Reversible<SeqNum>> const & s)
		{ return last_segment(s, g); });
}

inline auto out_segments(Reoriented<NodeNum> const & n, Graph const & g)
	// returns a range of Reoriented<Reversible<SegmentInSequence>>
{
	return out_sequences(n, g) | transformed(
		[&](Reoriented<Reversible<SeqNum>> const & s)
		{ return first_segment(s, g); });
}

// at

inline Position const & at(PositionInSequence const i, Graph const & g)
{
	return g[i.sequence][i.position];
}

inline Position at(Location const & l, Graph const & g)
{
	return between(at(from(l.segment), g), at(to(l.segment), g), l.howFar);
}

inline Position at(Reoriented<Location> const & l, Graph const & g)
{
	return l.reorientation(at(*l, g));
}

inline Position at(Reoriented<PositionInSequence> const & s, Graph const & g)
{
	return s.reorientation(at(*s, g));
}

inline V3 at(Reoriented<PositionInSequence> const & s, PlayerJoint const j, Graph const & g)
{
	return apply(s.reorientation, at(*s, g), j);
}

// misc

vector<Reoriented<SegmentInSequence>>
	neighbours(Reoriented<SegmentInSequence> const &, Graph const &, bool open);

inline Reoriented<NodeNum> const * node(Graph const & g, PositionInSequence const pis)
{
	if (pis.position.index == 0) return &g[pis.sequence].from;
	if (!next(pis, g)) return &g[pis.sequence].to;
	return nullptr;
}

inline Reoriented<NodeNum> const * node(Graph const & g, Location const loc)
{
	if (optional<PositionInSequence> const pis = position(loc))
		return node(g, *pis);
	
	return nullptr;
}

inline void replace(Graph & graph, PositionInSequence const pis,
	PlayerJoint const j, V3 const v, Graph::NodeModifyPolicy const policy)
{
	Position p = at(pis, graph);
	p[j] = v;
	graph.replace(pis, p, policy);
}

pair<vector<Position>, Reoriented<NodeNum>> follow(
	Graph const &, Reoriented<NodeNum> const &,
	SeqNum, unsigned frames_per_pos);

Reoriented<NodeNum> follow(Graph const &, Reoriented<NodeNum> const &, SeqNum);

Reoriented<Step> follow2(Graph const &, ReorientedNode const &, SeqNum);

NodeNum follow(Graph const &, NodeNum, SeqNum);

bool connected(Graph const &, NodeNum, NodeNum, bool no_tap = false);

inline optional<NodeNum> node_at(Graph const & g, PositionInSequence const pis)
{
	if (pis == first_pos_in(pis.sequence)) return *g[pis.sequence].from;
	if (pis == last_pos_in(pis.sequence, g)) return *g[pis.sequence].to;
	return boost::none;
}

inline optional<Reoriented<NodeNum>> node_at(Graph const & g, Reoriented<PositionInSequence> const pis)
{
	if (*pis == first_pos_in(pis->sequence)) return from(sequence(pis), g);
	if (*pis == last_pos_in(pis->sequence, g)) return to(sequence(pis), g);
	return boost::none;
}

inline optional<Reoriented<NodeNum>> node(Graph const & g, Reoriented<Location> l)
{
	if (auto p = position(l)) return node_at(g, *p);
	return boost::none;
}

set<NodeNum> nodes_around(Graph const &, set<NodeNum> const &, unsigned depth = 1, bool no_tap = false);

set<NodeNum> grow(Graph const &, set<NodeNum>, unsigned depth);

optional<SeqNum> seq_by_arg(Graph const &, string const & arg);
optional<NodeNum> node_by_arg(Graph const &, string const & arg);

inline bool is_sweep(Graph const & g, SeqNum const s)
{
	return g[s].from.reorientation.swap_players != g[s].to.reorientation.swap_players;
}

inline auto inout_sequences(Reoriented<NodeNum> const & n, Graph const & g)
{
	return g[*n].in_out | connector(n, g);
}

inline auto joint_positions(Reoriented<SeqNum> const & s, PlayerJoint const j, Graph const & g)
{
	return positions(s, g) | transformed(
		[&g, j](Reoriented<PositionInSequence> const & p) { return at(p, j, g); });
}

}

#endif
