/* This is a dummy header file used for documentation purposes with
 * Doxygen.  There is certainly a better way to do it; I'm just not
 * quite sure what that better way is. */
 
/** @mainpage
    
    Welcome to Djinni 2.2.1!  We wish you well, or some
    efficiently-computed close approximation thereof.
    
    Djinni is (c) 2004-2007 the University of Iowa, Jeff Ohlmann, Barrett
    Thomas, Robert Hansen and Tristan Thiede.
    
    Djinni is open source software.  You do not need to agree to any
    license agreement to use Djinni, but modifications and 
    redistributions are contingent upon your acceptance of the 
    licenses.  For more information, please see the \ref licenses.
    
    For an introduction to Djinni and what it does, please see the 
    \ref yap page.
    
    For information about installing Djinni, please see the \ref install
    page.
    
    For information about hacking on Djinni, please see the \ref hacking
    page.

    For information about the future of Djinni, please see the \ref future
    page.
    
    @page yap Introduction    
    @section dev_history Development
    @section history History
    
    Djinni is a set of C++ templates, source code and Python bindings
    which implements various things of use in the field of heuristic 
    research.  It was originally developed as a testbed for the 
    Ohlmann-Thomas compressed annealing algorithm, and over time grew to 
    be a general framework supporting annealing methods, direct 
    computation, straight hill-climbing algorithms, and even the 
    beginnings of genetic algorithms.
    
    Unfortunately, in the never-ending war against code cruft we realized
    that we were adding cruft in the name of limiting it.  Djinni 1.0 and 
    1.1 suffered from this, but with 2.0 we undertook a major 
    rearchitecturing to clean up the code.
    
    It almost succeeded.  The limitations of 2.0 led us back to the 
    drawing board for our 2.1 release, where we ditched much of the less 
    interesting heuristics and instead focused on annealing methods.
    
    @subsection current Today
    
    Djinni is a set of C++ templates, source code and Python bindings to 
    support the use of various kinds of annealing algorithms in an 
    educational environment.  It aims to be efficient, clean, and easy 
    to understand.  We hope it will be useful to students not only for the 
    study of annealing methods in heuristic research, but as an example of 
    a clean and extensible software architecture.
    
    Djinni is <i>small,</i> weighing in at only about a thousand lines of
    code (and considerably more than that in documentation).  We have 
    deliberately avoided design decisions which would have increased its
    size.  We understand that most of the UI students assigned to hack on
    Djinni are not computer science students, and for that reason we've
    done our level best to keep the codebase small enough so that a 
    student can keep the entire thing in their heads at once.  We are 
    proud of Djinni's small size.  When we started a couple of years ago 
    it was hovering around 5000 lines of unmaintainable, inextensible, 
    brittle and slow code.  Today it's a fifth the size, an order of 
    magnitude faster, and far easier to extend.
    
    We hope Djinni will help you in your studies here at UI, and that you
    have as much fun learning it as we did in making it.
    
    @section djinni_name What's a Djinni?
    
    Robin Williams as a big blue cartoon guy living in a magic lamp, for
    starters.  It's been Latinized, badly, as "genie".  Early in
    development one of the developers was certain his computer was 
    infested with a malevolent prankster that was making all the code 
    malfunction.  From there it was pretty easy to start 
    anthropomorphizing bugs as Robin Williams comedy routines, and thus 
    the name.
    
    @section concepts Basic Concepts
    
    Djinni's annealing algorithms are built around two concepts: that of
    the <i>penalty function</i> and the <i>solution type</i>.  In its
    broadest outlines, you create a penalty function object and a solution
    object, you pass them to the annealer, and then you call 
    Annealer.solve().  From that point it's just a simple matter of
    getting answers out of it.
    
    For instance, in C++:
    
    @code
    #include "Annealer.h"
    #include "Penalties.h"
    #include "TSPRoute.h"
    
    int main()
    {
      // Create a new solution out of a solution space defined by a Dumas 
      // dataset
      TravelingSalesman tsp("Dumas-1.set");
      // Create a new Simulated penalty function
      Simulated sim();
      // Create a new Annealer out of the two
      Annealer<Simulated, TravelingSalesman> annealer(sim, tsp);
      // Approximate a solution to the Traveling Salesman
      annealer.solve();
      // Output the solution
      cout << annealer << endl;
      // ... and finally, return 0 and end.
      return 0;
    }
    @endcode
    
    In Python it's almost as easy.  It's complicated somewhat by the
    fact Python doesn't understand C++ template syntax, but we manage
    to make do.  To see how C++ template types correspond with Python
    types, please see the file swig.i.
    
    @verbatim
    from Djinni import TSP, SA_TSP, Simulated
    tsp = TSP("Dumas-1.set")
    sim = Simulated()
    annealer = SA_TSP(sim, tsp)
    annealer.solve()
    print annealer.solution() @endverbatim
    
    @section wtfmatebbq Why Python?
    
    Because Python is t3h h4ppy.  Because Python makes us smile.  Because
    Python is a lot friendlier to program in than C++.  And because we
    just think it's groovy.

    @page future Mysterious Future
    @section planned When We Make Plans, God Laughs

    At present, the future of Djinni looks ... slow.  This is not a high 
    priority subject for either of the two main developers, since it's not
    related to our graduate studies.  Some people in the Henry B. Tippie
    School of Business Management are interested in extending it, but any
    future developments will be slow and incremental.

    However, if you're interested in helping Djinni, here's a list of
    some tasks we'd like to see implemented.  Some are large, and some are
    much more bite-sized.

    - Large:
        - Implement a parameterized genetic search algorithm
        - Develop a framework for hybrid algorithms
    - Medium:
        - Write better documentation
        - Polish the SWIG bindings
        - Explore performance improvements
        - Hack the build system to allow people to choose between
          <tt>boost::shared_ptr</tt> and <tt>std::tr1::shared_ptr</tt>
          at build-time
        - Replace our Twister code with the Mersenne Twister from 
          <tt>&lt;tr1/random&gt;</tt>.  Many compilers do not yet support
          this header.  Please do not remove the Twister code until you're
          sure that <tt>&lt;tr1/random&gt;</tt> is supported on all the
          major platforms.
            - Of course, for real geek points you could hack the build
              system to automagically detect this.
    - Small:
        - Come up with unit tests for Djinni.  Right now we have only one,
          and we really need more.
    
    @page install Install
    @section beforeyoubegin Before You Begin...
    
    We don't like Autotools, but we have to admit that it's probably
    the best game in town right now.  In the past we tried a CMake-based
    build system, but that turned out to create more problems than it
    solved.
    
    That means you can do a simple <tt>./configure && make && make
    install</tt> dance to install Djinni.  Depending on your system you
    may need to use the <tt>--prefix</tt> and/or <tt>--with-boost</tt>
    arguments to <tt>./configure</tt>.

    You will need:
    
    - Python 2.2 or later
        - Djinni can probably be downported as far as Python 1.5.2, but we
          haven't tried it.  Please use 2.2 or later.
    - GCC 3.0 or later.
        - GCC 3.x will use Boost libraries for some of Djinni.  GCC 4.x
          will use <tt>&lt;tr1/memory&gt;</tt> instead of Boost's 
          <tt>&lt;boost/shared_ptr.hpp&gt;</tt>.
    - MSVC++ 7.0 and later <i>may</i> work.  (If it works for you, please
        let us know!)
	    - You will need Boost.
	- Boost 1.34 or later
    - SWIG 1.3.20 or later
        - It can probably be downgraded to the early 1.3 series or 
          even earlier, but we haven't tried it.
    - Doxygen 1.5.2 or later
    - Graphviz 2.0 or later
    
    Most installation troubles stem from not having the proper tools.
    Please make sure you're using the proper tools before you file a bug
    report.  

    @section building Building the Perfect Beast
    @subsection apologies (... with apologies to Don Henley)
    
    Once you've uncompressed Djinni into its own directory, it should just
    be a simple process of typing 

    <tt>./configure --prefix=</tt><i>install-path</i> 
    <tt>--with-boost=</tt><i>boost-root</i> 
    <tt>&amp;&amp; make &amp;&amp; make install</tt>

    The process of building takes well under a minute on modern
    hardware.
    
    Once the build completes and the installation finishes, you can
    find the Python libraries in the appropriate <tt>site-packages</tt>
    directory.  E.g., if I used <tt>/export/home/rjh</tt> as my
    prefix and I'm running Python 2.5, I could find the libraries in 
    <tt>/export/home/rjh/lib/python2.5/site-packages/djinni</tt>.
    
    Make sure your PYTHONPATH environment variable is set correctly.
    E.g., using the bash shell I would type:
    
    <tt>export
    PYTHONPATH=/export/home/rjh/lib/python2.5/site-packages</tt>
    
    Once your PYTHONPATH variable is set, feel free to pop into 
    <tt>src/examples</tt> and run <tt>gtk-salesman.py</tt>.  If this
    application runs successfully, you can be confident Djinni is
    installed correctly.
        
    @page hacking Hacking
    
    We sincerely hope you will find hacking on Djinni to be a pleasant 
    and rewarding experience.
    
    Djinni's annealers use C++ in fairly standard industry idioms.  You
    will need to be familiar with the basic concepts of object oriented
    analysis and design, as well as the use of templates and generic
    programming, particularly partial template specialization.
    
    We will not be covering the Annealer constructors, member functions
    or other details here.  The reader is referred to the API
    documentation for those matters.
    
    @section bring_your_own_tools Tools Necessary
    
    In addition to the tools required for compiling Djinni, you will also
    need the GNU Autotools toolchain.  Please use the most recent version
    available.  In hacking Djinni, we use:
    
    - The GNU toolchain (Autoconf, Automake, Libtool, etc.)
    - Graphviz 2.12
    - Doxygen 1.5.2
    - G++ 4.1.2
    - SWIG 1.3.31
    - Python 2.5
    - Boost 1.34
    
    @section annealer_hacking Hacking the Annealer
    
    An annealer is parameterized by its penalty function and the kind
    of solution upon which it's working.  Penalty functions govern the
    behavior of the lambda term in the annealing equasion.  Solutions
    represent hyperplanes within larger solution spaces.
    
    Since these are template parameters, there is no PenaltyFunc 
    superclass, nor any SolutionType superclass.  Something is a 
    PenaltyFunc if it passes the duck test--if it quacks like a duck, 
    flies like a duck and looks like a duck, it's a duck.
    
    @subsection penaltyfunc_hacking Hacking Annealer Penalty Functions
    
    A PenaltyFunc must make a public typedef:
    - <tt>typedef</tt> <i>foo</i> <tt>ReturnType;</tt>
    
    ... substituting your own lambda term's value type for <i>foo</i>, of 
    course.  This typedef is used by Annealer to know what type of data
    to expect from the lambda term.  Almost always your PenaltyFunc will
    yield a value of type <tt>double</tt>, but we see no reason to confine
    you to that.  If your PenaltyFunc needs to yield a complex custom data
    type, you can do that just fine, as long as it has basic mathematical
    operators available for it.
    
    A PenaltyFunc must implement a method signature compatible to:
    
    - <tt>ReturnType operator()(uint32_t)</tt>
    
    ... by 'compatible to' we mean that it can accept any parameter, as 
    long as an implicit conversion exists between <tt>uint32_t</tt> and 
    its parameter type.  For instance, you could have <tt>operator()(const 
    double) </tt>, since an implicit conversion exists from <tt>int</tt> 
    to <tt>const double</tt>.
    
    If your PenaltyFunc implements that one typedef and that one method
    signature, then it can be used as a PenaltyFunc with Annealer.
    
    @subsection solutiontype_hacking Hacking Annealer Solution Types
    
    A SolutionType has a lot of responsibilities.  It must keep track of 
    its solution, offer facilities for generating a new SolutionType which 
    adjoins itself in the solution space, provide accessors for the 
    feasible and infeasible costs of the solution, and other details.
    
    Keeping track of the solution space is entirely your responsibility.
    Djinni is entirely agnostic on how solution spaces should be 
    represented.  Our provided sample SolutionType, TSPRoute, shares a 
    pointer to a data structure representing the solution space for an 
    instance of the Traveling Salesman Problem.  Please do not think you 
    must do likewise. Please do not confuse architectural <i>choice</i> 
    with architectural <i>necessity</i>.
    
    With that said, TSPRoute is close to the minimal set of methods which
    will pass the duck test for a SolutionType.  To wit, SolutionTypes 
    must provide methods with signatures compatible to:
    
    - <tt>void setF(double)</tt>
    - <tt>void setP(double)</tt>
    - <tt>double getF() const</tt>
    - <tt>double getP() const</tt>
    - <tt>void generateNeighbor(SolutionType&)</tt>
    - <tt>void update()</tt>
    - <tt>void randomize()</tt>
    - <tt>void compute()</tt>
    
    Of these, please note that <tt>update</tt> and <tt>compute</tt> may 
    very well be empty functions for certain kinds of solutions.  However, 
    given that annealing is inherently stochastic, we strongly recommend 
    your <tt>randomize</tt> method do what's expected.
    
    Additionally, SolutionTypes must have 
    <tt>std::ostream& operator<<(std::ostream&, const SolutionType&)</tt>
    overloaded appropriately for them.  Annealer uses streams to create
    human-readable output of solutions.
    
    
    @section partial_template_specialization Partial Template Specialization
    
    Some PenaltyFuncs may require Annealer to handle things slightly 
    differently.  If this is the case, make a partial template 
    specialization, as we did for Annealer<Compressed, SolutionType>.
    
    
    @section some_assembly_required Putting It All Together
    
    If you're writing a new penalty function, please consider adding it to
    the file Penalties.h .  Typically penalty functions are small and 
    terse, making them easy to put in one single header file.
    
    If you're writing a new solution type, please consider making a new
    subdirectory off <tt>src/</tt> and put it in there.
    
    <b>You will need to understand Autotools to hack on the build
    system.</b>  Autotools is a nontrivial piece of software.  Please
    ask your local Autotools guru for help.
    
    Once this is done, it's time to hack the <tt>swig/swig.i</tt> file.
    You may want to refer to the SWIG documentation for this.  The 
    hand-holding here is not meant, in any way, to replace the SWIG
    documentation.
    
    The first line you see, <tt>\%module</tt>, just gives the name by
    which the resulting Python module will be known.  If you've got 
    Djinni installed successfully and you want to leave your existing
    Djinni alone while you further develop it, you may want to rename
    this <tt>Djinni_dev</tt>.  (Or <tt>Elvis</tt>, for all we care.  Hail
    to the King, baby.)
    
    The second thing you see, <tt>\%{ ... \%}</tt>, encapsulates a list
    of header files.  This is C++ code which SWIG must insert into its own
    self-generated C++ code in order to work properly.  As a rule of 
    thumb, if you're going to have SWIG generate Python bindings for an 
    object, you need to <tt>\#include</tt> the header file containg the 
    definition for that object.
    
    The third thing you see are <tt>\%include</tt> statements.  These
    are commands to SWIG, telling it to read through those files and 
    generate bindings for everything it sees.  SWIG also comes with 
    some libraries that are very useful, such as automagically converting
    std::strings into Python strings: those, too, are %included here.
    
    Finally, you'll see some <tt>\%template</tt> parameters.  These
    are necessary because of an irreconcilable difference between C++
    and libraries.  C++ templates exist in header files, not libraries.
    There is no executable code associated with a template; a template
    is just a set of rules telling the compiler how to generate executable
    code in the event it's needed.  This is problematic if you're trying 
    to create a library for Python to call out to!
    
    Thus, we need to explicitly tell SWIG to instantiate a few templates.
    <tt>\%template(</tt><i>foo</i><tt>)</tt> <i>bar;</i> tells SWIG
    to instantiate the fully parameterized template class <i>bar</i> and
    to make that available within the generated Python library under the
    name <i>foo</i>.
    
    As of SWIG 1.3.17, SWIG is a little touchy with respect to using
    typedefed names.  For instance, TravelingSalesman is a typedef for
    TSPRoute<TSPTWWorld>; yet, if we try to <tt>\%template(TSP) 
    TravelingSalesman</tt>, SWIG will complain about a syntax error.
    Using the typedefed name as a parameter type, as we do with the
    <tt>CA_TSP</tt> and <tt>SA_TSP</tt> template instantiations, causes
    no error.  If you're sure your code is fine but SWIG is still 
    complaining, try avoiding all typedefs in the base name and its
    parameterization.
        
    @section tobacco_is_bad_for_you Rolling Your Own (Package, That Is)
    
    Once all of this is done, you'll be ready to go.  First, enter the 
    directory above <tt>src/</tt> (i.e., where you uncompressed Djinni)
    and clean up your tree with <tt>make clean</tt>.  Then do a dance of
    <tt>aclocal -I m4 &amp;&amp; autoconf &amp;&amp; automake &amp;&amp;
    libtoolize</tt>.  From there you can reconfigure, remake, and 
    reinstall.    

    @section houston_we_have_a_problem If all else fails...
    
    If all else fails, you can email the core Djinni developers at
    rjh@sixdemonbag.org or tthiede@thetiredsaint.com.  If our schedules
    permit, we'd be happy to help you with your hacking and packaging.
    However, we don't work for free.  For UI students, we typically charge
    a good meal at the Summit and a cold beer to wash it down afterwards.
*/

/**
    @page licenses License Agreements
    
    Djinni is (c) 2006, the University of Iowa, Jeff Ohlmann, Barrett 
    Thomas, Robert J. Hansen, Tristan Thiede.  All rights reserved.  Code 
    and documentation are released under separate licenses.  For purposes 
    ofthese licenses, "documentation" shall incorporate Doxygen comments 
    made in files which would otherwise be considered to be code.
    
    @section codelicence Code License
    
    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are 
    met:
    
    - Redistributions of source code must retain the above copyright 
      notice, this list of conditions and the following disclaimer.

    - Redistributions in binary form must reproduce the above copyright 
      notice, this list of conditions and the following disclaimer in the 
      documentation and/or other materials provided with the distribution.
      
    - Neither the name of the University of Iowa nor the names of its
      contributors may be used to endorse or promote products derived from 
      this software without specific prior written permission.
      
<b>THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, 
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR 
PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR 
CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, 
EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, 
PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR 
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.</b>
    
    @section documentationlicense Documentation License
    
    Djinni's documentation, including these Doxygen pages, are released 
    under a Creative Commons Attribution-Sharealike 3.0 License.  You are 
    free to copy, distribute, display and even perform this documentation 
    (although we can't imagine why you'd want to do an interpretive dance 
    of it), provided you give proper attribution in the form of preserving 
    all copyright notices and that any alterations, transformations or 
    derivative works will be distributed only under a Creative Commons 
    Attribution-Sharealike 3.0 License.
    
    For any reuse or distribution you must make clear to others these 
    license terms.
    
    This is a human-readable summary of a very legalistic document found 
    at http://creativecommons.org/licenses/by-sa/3.0/legalcode .  In case 
    of any conflict between this human-readable summary and the legal 
    document, the legal document will govern.    
*/
