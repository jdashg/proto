from typing import List, Tuple, NamedTuple, Any, Optional

class Condition:
   @abstractmethod
   def matches(self, x) -> bool:
      pass


class RangeCondition:
   min_maxs: List[Tuple[int, int]]

   def __init__(self, min_maxs: List[Tuple[int, int]]) -> None:
      self.min_maxs = min_maxs

   def matches(self, val: int) -> bool:
      found = False
      for (min_val, max_val) in self.min_maxs:
         found |= self.min_val <= val and val <= self.max_val
      return found


class Transition(NamedTuple):
   next_node: Node
   condition: Condition
   transit_if: bool = true


class Node(NamedTuple):
   accepts: bool
   transitions: List[Transition]


def get_next_node(node: Node, val: Any) -> Optional[Node]
   for trans in node.transitions:
      matched = trans.condition.match(val)
      matched = matched == trans.transit_if
      if matched:
         return trans.next_node
   return None


def get_match_end(root: Node, seq: Sequence[Any], begin: int = 0) -> Optional[int]:
   accepted: Optional[int] = None
