#pragma once

#include <Parsers/IAST_fwd.h>
#include <Interpreters/Context_fwd.h>
#include <Analyzer/IQueryTreePass.h>


namespace DB
{

/** This pass make initial query analysis.
  *
  * 1. All identifiers are resolved. Next passes can expect that there will be no IdentifierNode in query tree.
  * 2. All matchers are resolved. Next passes can expect that there will be no MatcherNode in query tree.
  * 3. All functions are resolved. Next passes can expect that for each FunctionNode its result type will be set, and it will be resolved
  * as aggregate or non aggregate function.
  * 4. All lambda expressions that are function arguments are resolved. Next passes can expect that LambaNode expression is resolved, and lambda has concrete arguments.
  * 5. All standalone lambda expressions are resolved. Next passes can expect that there will be no standalone LambaNode expressions in query.
  * 6. Constants are folded. Example: SELECT plus(1, 1). After step will be: SELECT 2.
  * Motivation for this, there are places in query tree that must contain constant:
  * Function parameters Example: SELECT quantile(0.5)(x).
  * Functions in which result type depends on constant expression. Example: cast(x, 'type_name').
  * Expressions that are part of LIMIT. Example: SELECT * FROM test_table LIMIT expr.
  *
  * 7. All scalar subqueries are evaluated.
  * TODO: Scalar subqueries must be evaluated only if they are part of query tree where we must have constant. This is currently not done
  * because execution layer does not support scalar subqueries execution.
  *
  * 8. Special functions handling:
  * Function `untuple` is handled properly.
  * Function `arrayJoin` is handled properly.
  * For function `grouping` arguments are resolved, but it is planner responsibility to initialize it with concrete grouping function
  * based on group by kind and group by keys positions.
  * For function `in` and its variations arguments are resolved, but sets are not build. If left and right arguments are constants
  * constant folding is performed.
  */
class QueryAnalysisPass final : public IQueryTreePass
{
public:
    String getName() override
    {
        return "QueryAnalysis";
    }

    String getDescription() override
    {
        return "Resolve type for each query expression. Replace identifiers, matchers with query expressions. Perform constant folding. Evaluate scalar subqueries.";
    }

    void run(QueryTreeNodePtr query_tree_node, ContextPtr context) override;

};

}
